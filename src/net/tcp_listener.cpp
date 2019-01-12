#include "prefixhead.h"
#include "tcp_listener.h"
// #include "NetTCPConnection.h"
// #include "NetPacketParser.h"
// #include "NetWorker.h"
// #include "IOModel.h"
// #include "IOModelEpoll.h"
// #include "IOModelKqueue.h"
// #include "NetModule.h"

CTCPListener::CTCPListener(void)
: packet_parser_(NULL)
, session_creator_(NULL)
{
	//LOG_DBG("CTCPListener(void)");
}

CTCPListener::~CTCPListener(void)
{
	//LOG_DBG("~CTCPListener(void)");
}

void CTCPListener::release()
{
	stop_listen();
	delete this;
}

void CTCPListener::set_parser(IPacketParser* parser)
{
	packet_parser_ = parser;
}

void CTCPListener::set_creator(ISessionCreator* creator)
{
	session_creator_ = creator;
}

bool CTCPListener::start_listen(const char* listen_addr, int port)
{
	// //1.create socket
	// fd_ = _create_socket(listen_addr, port, SOCK_STREAM);
	// if (fd_ < 0)
    // {
    //     LOG_ERR("failed to create socket! err: %s", p_socket_last_error());
    //     return false;
    // }

    // //3.modify socket
	// CConnection::set_reuse_port(fd_);
    
    // //4. bind
	// if (0 != ::bind(fd_, addrinfo_res_->ai_addr, addrinfo_res_->ai_addrlen))
	// {
	// 	LOG_ERR("failed to bind! err: %s", p_socket_last_error());
	// 	return false;
	// }

	// LOG_DBG("start listen to [%s|%d]", listen_addr, port);

    // //5. listen
	// if (0 != ::listen(fd_, 128))
    // {
	// 	LOG_ERR("failed to listen[%s|%d]! err: %s", listen_addr, port, p_socket_last_error());
	// 	return false;
	// }

	// CNetWorker::GetInstancePtr()->AddSocket(fd_, EPOLLIN, (void*)this);
    
	return true;
}

void CTCPListener::stop_listen()
{
	// if (fd_ != -1)
	// {
	// 	CNetWorker::GetInstancePtr()->DelSocket(fd_);
	// 	CLOSE_SOCKET(fd_);
	// 	fd_ = -1;
	// }
}

void CTCPListener::on_accept()
{
	// struct sockaddr_in6 client_addr;
	// memset(&client_addr, 0, sizeof(client_addr));
	// socklen_t addrlen = sizeof(client_addr);
	// int fd_conn = ::accept(fd_, (sockaddr*)&client_addr, &addrlen);
	// if (fd_conn == -1)
	// {
	// 	LOG_ERR("failed to accept, err: %s", p_socket_last_error());
	// 	return;
	// }

	// {//print
	// 	//char ip_addr[INET6_ADDRSTRLEN] = {0};
	// 	//inet_pton(AF_INET6, ip_addr, &client_addr.sin6_addr);
	// 	//LOG_DBG("accept client[%s|%d], fd: %d", ip_addr, ntohs(client_addr.sin6_port), fd_conn);

	// 	LOG_DBG("accept client[%s|%d], fd: %d"
	// 		, inet_ntoa(((struct sockaddr_in *)&client_addr)->sin_addr)
	// 		, ntohs(((struct sockaddr_in *)&client_addr)->sin_port)
	// 		, fd_conn);
	// }

	// if (!session_creator_->OnPreAccept((struct sockaddr*)&client_addr))
	// {
	// 	CLOSE_SOCKET(fd_conn);
	// 	LOG_ERR("failed to accept preparatively!");
	// 	return;
	// }

	// CNetTCPConnection* connection = NET_POOL->PopNetTCPConnection();
	// if (!connection)
	// {
	// 	CLOSE_SOCKET(fd_conn);
	// 	LOG_ERR("failed to get tcp connection!");
	// 	return;
	// }

	// IYiSession* session = session_creator_->OnCreateSession();
	// if (!session)
	// {
	// 	CLOSE_SOCKET(fd_conn);
	// 	LOG_ERR("failed to create session!");
	// 	return;
	// }

	// CConnection::set_nonblocking(fd_conn);
	// connection->SetSession(session);
	// connection->SetSockPacketParser(packet_parser_);
	// connection->_OnConnection(fd_conn);
}

