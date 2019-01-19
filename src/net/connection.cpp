#include "prefixhead.h"
#include "connection.h"
#include "unprtt.h"
#include "net_module.h"
#include "net_event.h"
#include "packet_parser.h"

CConnection::CConnection()
: is_client_(false)
, connect_status_(UNCONNECT)
, recv_buffer_(NULL)
, recv_offset_(0)
, remote_port_(0)
{
}

CConnection::~CConnection()
{

}

void CConnection::set_parser(IPacketParser* parser)
{
	if (parser == NULL)
	{
		static CPacketParser parse__;
		packet_parser_ = &parse__;
	}
	else
	{
		packet_parser_ = parser;
	}
}
	
bool CConnection::send(const IPacket *packet)
{
	return this->send(packet->data(), packet->length());
}

bool CConnection::send(const char *data, int len)
{
	if(is_client_ && !is_connected())
	{
        //ReConnect();
        LOG_ERR("failed to send, not connected!");
		return false;
	}
    
	if(sock_fd_ == -1)
	{
		LOG_ERR("failed to send, socket invalid!");
		return false;
	}

	CTCPEvent* ev = get_module()->get_pool()->pop_tcpvent();
	if(!ev)
    {
        LOG_EXP("CNetTCPEvent pool is over!");
		return false;
    }
	
	if (!packet_parser_)
	{
		LOG_ERR("no packet parser!");
		return false;

		// ev->set(NULL, data, len);
		// que_of_send_buffer_.push(ev);
		// get_module()->get_reactor()->modify_epoll_event(this, EPOLLIN | EPOLLOUT);
		// return true;
	}

	char tmp_buf[MAX_OVERLAP_BUFFER] = {0};
	int tmp_len = MAX_OVERLAP_BUFFER;
	packet_parser_->encode(data, len, tmp_buf, tmp_len);
	ev->set(NULL, tmp_buf, tmp_len);
	
	que_of_send_buffer_.push(ev);
	get_module()->get_reactor()->modify_epoll_event(this, EPOLLIN | EPOLLOUT);
    
	return true;
}

void CConnection::close_connection()
{
	clear_send_queue();

	CTCPEvent* ev = get_module()->get_pool()->pop_tcpvent();
	if(!ev)
	{
        LOG_EXP("CTCPEvent pool is over!");
        return;
	}
    ev->set(this, NULL, (int)ON_CLOSECONNECTION);
    ev->set_delay(helper::CUtility::GetCurTime() + 1500);
	get_module()->get_reactor()->push_delayevent(ev);
}

void CConnection::reconnect()
{
	connect(remote_addr_, remote_port_);
}

void CConnection::get_addr(struct sockaddr* remoteaddr)
{
	socklen_t len = sizeof(struct sockaddr);
	if (0 != ::getpeername(sock_fd_, remoteaddr, &len))
	{
		LOG_ERR("err: %s", p_socket_last_error());
	}
}

bool CConnection::connect(const char* addr, int port)
{
	strncpy(remote_addr_, addr, sizeof(remote_addr_));
	remote_port_ = port;

	CTCPEvent* ev = get_module()->get_pool()->pop_tcpvent();
	if (!ev)
	{
		LOG_EXP("CTCPEvent pool is over!");
		return false;
	}

	ev->set(this, NULL, (int)ON_CONNECT);
	get_module()->get_reactor()->push_event(ev);

	return true;
}

void CConnection::on_connect()
{
    if(connect_status_ != UNCONNECT)
        return;
	
	connect_status_ = CONNECTING;
    is_client_ = true;

	// 1.creater socket
	if (!INetSocket::socket_create(remote_addr_, remote_port_, SOCK_STREAM))
	{
		LOG_ERR("failed to create socket! ret=%d, err: %s", sock_fd_, p_socket_last_error());
		return;
	}
        
    LOG_DBG("start connect to [%s:%d]", remote_addr_, remote_port_);

	// 2.connect
	bool ret = INetSocket::socket_connect();
	if (!ret)
	{
		LOG_ERR("failed to connect[%s:%d]! err: %s", remote_addr_, remote_port_, p_socket_last_error());
	}

    get_module()->get_reactor()->add_epoll_event(this, EPOLLOUT);
    INetSocket::set_nonblocking(sock_fd_);

    if(ret)
    {
        _on_connection(sock_fd_);
    }
    else
    {
        if(errno != EINPROGRESS)
        {
            _on_disconnect();
		}
    }
}

void CConnection::on_connection()
{
	if(session_)
	{
		session_->on_connection(this);
	}
}

void CConnection::on_disconnect()
{
	if(session_)
	{
		session_->on_disconnect();
	}
}

void CConnection::on_disconnection()
{
	if(session_)
	{
		session_->on_disconnection();
		session_->release();
	}

	if(!is_client_)
	{
		get_module()->get_pool()->push_connection(this);
		is_client_ = false;
	}
}

void CConnection::on_closeconnection()
{
	
}


void CConnection::on_recv()
{
	// TODO: 待优化-"为了兼容多种编解码器，导致多次内存拷贝"，如:
	// 1.recv拷贝
	// 2.parser拷贝
	// 3.push队列拷贝
	// ***

	if (!recv_buffer_)
	{
		recv_buffer_ = static_cast<char*>(get_module()->get_memory()->Malloc(MAX_OVERLAP_BUFFER));
		if (!recv_buffer_)
		{
			LOG_EXP("memory is not enough!");
			return;
		}
	}

	int len = INetSocket::socket_recv(recv_buffer_, MAX_OVERLAP_BUFFER - recv_offset_);
	if(len <= 0)
	{
		if(len == -1 && (errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN) )
		{
			LOG_WAR("recv invalid len: %d!", len);
			return;
		}

		int err = 0;
		std::string error(p_socket_last_error(&err));
		LOG_ERR("failed to recv! len: %d, err: %s", len, error.c_str());
		_on_disconnection();
		return;
	}
	recv_offset_ += len;

	while (true)
	{
		if(recv_offset_ <= 0)
			return;

		if (!packet_parser_)
		{
			LOG_ERR("no packet parser!");
			recv_offset_ = 0;
			return;
		}

		packet_.clear();

		int max_len = MAX_OVERLAP_BUFFER;
		int out_size = packet_parser_->decode(&packet_, max_len, recv_buffer_, recv_offset_);
		if (out_size <= 0)
		{
			LOG_INF("incomplete packet! size=%d", out_size);
			return;
		}

		recv_offset_ -= out_size; 
		if(recv_offset_ > 0)
		{
			// TODO: to use ring buffer
			memcpy(recv_buffer_, recv_buffer_ + out_size, recv_offset_);
		}
			
		if(max_len <= 0 || max_len >= MAX_OVERLAP_BUFFER)
		{
			LOG_WAR("invalid max_len: %d !", max_len);
			return;
		}

		CTCPEvent* ev = get_module()->get_pool()->pop_tcpvent();
		if(ev)
		{
			ev->set(this, packet_.data(), packet_.length());
			get_module()->main_event_queue()->push(ev);
		}
	}
}

void CConnection::on_send()
{
	if(is_client_ && connect_status_ != CONNECTED)
	{
		sockaddr_in6 peeraddr;
		memset(&peeraddr, 0, sizeof(peeraddr));
		socklen_t len = sizeof(sockaddr_in6);
		int ret = ::getpeername(sock_fd_, (sockaddr*)&peeraddr, &len);
		if(ret < 0)
        {
        	LOG_ERR("connect status! ret=%d, err: %d, %s", ret, errno, p_socket_last_error());
			_on_disconnect();
            return;
		}
		
        _on_connection(sock_fd_);
		return;
	}

	if(sock_fd_ == -1)
	{
		LOG_ERR("failed to OnSend, socket invalid!");
		return;
	}

	do
	{
		CTCPEvent* ev = que_of_send_buffer_.front();
		if(ev == NULL)
        {
			get_module()->get_reactor()->modify_epoll_event(this, EPOLLIN);
			return;
		}

		while (true)
		{
			int len = INetSocket::socket_send(ev->get_data(), ev->get_length(), MSG_NOSIGNAL);
			if (len == -1)
			{
				if(errno != EWOULDBLOCK)
					LOG_ERR("failed to send! err: %s", p_socket_last_error());
				return;
			}

			if(len < ev->get_length())
			{
				ev->transfer_data(len);
			}
			else
			{
				que_of_send_buffer_.pop();
				get_module()->get_pool()->push_tcpevent(ev);
				break;
			}
		}
	} while(que_of_send_buffer_.size() > 0);
}

void CConnection::on_error()
{
	LOG_ERR("on_error...");
}

void CConnection::_on_connection(int sockfd)
{
	sock_fd_ = sockfd;
    connect_status_ = CONNECTED;

	recv_offset_ = 0;
	clear_send_queue();
    
	if(is_client_)
		get_module()->get_reactor()->modify_epoll_event(this, EPOLLIN);
	else
		get_module()->get_reactor()->add_epoll_event(this, EPOLLIN);
    
	CTCPEvent* ev = get_module()->get_pool()->pop_tcpvent();
	if(!ev)
    {
        LOG_EXP("CTCPEvent pool is over!");
        return;
	}
    
    ev->set(this, NULL, (int)ON_CONNECTION);
    get_module()->main_event_queue()->push(ev);
}

void CConnection::_on_disconnect()
{
	if(sock_fd_ == -1)
	{
		LOG_ERR("invalid sock_fd_!");
		return;
	}
    
	connect_status_ = UNCONNECT;
	if(!get_module()->get_reactor()->del_epoll_event(this))
	{
		LOG_ERR("failed to delete socket! err: %s", p_socket_last_error());
	}
    
	INetSocket::socket_close();
    
	CTCPEvent* ev = get_module()->get_pool()->pop_tcpvent();
	if(!ev)
    {
        LOG_EXP("CTCPEvent pool is over!");
        return;
	}
    ev->set(this, NULL, (int)ON_DISCONNECT);
	get_module()->main_event_queue()->push(ev);
}

void CConnection::_on_disconnection()
{
	if (sock_fd_ == -1)
		return;

	connect_status_ = UNCONNECT;
	clear_send_queue();

	if (!get_module()->get_reactor()->del_epoll_event(this))
	{
		LOG_ERR("failed to delete socket! err: %s", p_socket_last_error());
	}
    INetSocket::socket_close();
    
	CTCPEvent* ev = get_module()->get_pool()->pop_tcpvent();
	if(!ev)
    {
        LOG_EXP("CTCPEvent pool is over!");
        return;
	}
    
    ev->set(this, NULL, (int)ON_DISCONNECTION);
    get_module()->main_event_queue()->push(ev);
}

void CConnection::clear_send_queue()
{
	while (que_of_send_buffer_.size())
	{
		CTCPEvent* ev = que_of_send_buffer_.pop();
		if (NULL == ev)
			continue;
        
		get_module()->get_pool()->push_tcpevent(ev);
	}
}