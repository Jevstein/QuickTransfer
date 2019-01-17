#include "prefixhead.h"
#include "connection.h"
// #include "unprtt.h"
#include "net_module.h"
#include "net_event.h"

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

bool CConnection::send(const char* data, int size)
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
	
    // char tmp_buf[MAX_OVERLAP_BUFFER] = {0};
	// int tmp_len = MAX_OVERLAP_BUFFER;
	// packet_parser_->encode(data, size, tmp_buf, tmp_len);
	// ev->set(NULL, tmp_buf, tmp_len);
	ev->set(NULL, data, size);
	
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
	char buf[1024] = {0};
	int len = INetSocket::socket_recv(buf, sizeof(buf));
	if(len <= 0)
	{
		int err = 0;
		std::string error(p_socket_last_error(&err));
		if (err != 10054 && err != 0)
		{
			LOG_ERR("failed to recv! len: %d, err: %s", len, error.c_str());
		}

		_on_disconnection();
		return;
	}

	CTCPEvent* ev = get_module()->get_pool()->pop_tcpvent();
	if(ev)
	{
		ev->set(this, buf, len);
		get_module()->main_event_queue()->push(ev);
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
			int len =  INetSocket::socket_send(ev->get_data(), ev->get_length());
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