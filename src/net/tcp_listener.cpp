#include "prefixhead.h"
#include "tcp_listener.h"
#include "connection.h"
// #include "NetTCPConnection.h"
// #include "NetPacketParser.h"
// #include "NetWorker.h"
// #include "IOModel.h"
// #include "IOModelEpoll.h"
// #include "IOModelKqueue.h"
#include "net_module.h"

CTCPListener::CTCPListener(void)
: packet_parser_(NULL)
, session_creator_(NULL)
{

}

CTCPListener::~CTCPListener(void)
{
	
}

void CTCPListener::release()
{
	stop_listen();
	delete this;
}

bool CTCPListener::start_listen(const char* listen_addr, int port)
{
	// 1.create socket
	if (INetSocket::socket_create(listen_addr, port, SOCK_STREAM) < 0)
	{
        LOG_ERR("failed to create socket! err: %s", p_socket_last_error());
        return false;
	}

	// *.set reuse
	if (!INetSocket::set_reuse_port())
	{
		LOG_ERR("failed to reuse port!");
		addrinfo_res_ = addrinfo_res_->ai_next;
		return false;
	}

	// 2.bind
	if (!INetSocket::socket_bind())
	{
		LOG_ERR("failed to bind! err: %s", p_socket_last_error());
		addrinfo_res_ = addrinfo_res_->ai_next;
		return false;
	}

	// 3.listen
	if (!INetSocket::socket_listen(128))
	{
		LOG_ERR("failed to listen[%s.%d]! err: %s", listen_addr, port, p_socket_last_error());
		addrinfo_res_ = addrinfo_res_->ai_next;
		return false;
	}

	get_module()->get_reactor()->add_socket(sock_fd_, EPOLLIN, (void *)this);
    
	return true;
}

void CTCPListener::stop_listen()
{
	if (sock_fd_ != -1)
	{
		get_module()->get_reactor()->del_socket(sock_fd_);
		INetSocket::socket_close();
	}
}

void CTCPListener::on_accept()
{
	struct sockaddr_in6 client_addr;
	int conn_fd = INetSocket::socket_accept(&client_addr);
	if (conn_fd < 0)
	{
		LOG_ERR("failed to accept, err: %s", p_socket_last_error());
		return;
	}

	if (!INetSocket::set_nonblocking(conn_fd))
	{
		LOG_ERR("failed to nonblocking!");
		CLOSE_SOCKET(conn_fd);
		return;
	}

	// {//print
	// 	//char ip_addr[INET6_ADDRSTRLEN] = {0};
	// 	//inet_pton(AF_INET6, ip_addr, &client_addr.sin6_addr);
	// 	//LOG_DBG("accept client[%s|%d], fd: %d", ip_addr, ntohs(client_addr.sin6_port), fd_conn);

	// 	LOG_DBG("accept client[%s|%d], fd: %d"
	// 		, inet_ntoa(((struct sockaddr_in *)&client_addr)->sin_addr)
	// 		, ntohs(((struct sockaddr_in *)&client_addr)->sin_port)
	// 		, fd_conn);
	// }

	if (!session_creator_->on_preaccept((struct sockaddr*)&client_addr))
	{
		LOG_ERR("failed to accept preparatively!");
		CLOSE_SOCKET(conn_fd);
		return;
	}

	CConnection* connection = get_module()->get_pool()->pop_connection();
	if (!connection)
	{
		LOG_ERR("failed to get tcp connection!");
		CLOSE_SOCKET(conn_fd);
		return;
	}

	ISession* session = session_creator_->on_create();
	if (!session)
	{
		LOG_ERR("failed to create session!");
		CLOSE_SOCKET(conn_fd);
		get_module()->get_pool()->push_connection(connection);
		return;
	}

	connection->set_session(session);
	connection->set_parser(packet_parser_);
	connection->_on_connection(conn_fd);
}

