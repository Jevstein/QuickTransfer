#include "prefixhead.h"
#include "connection.h"
// #include "unprtt.h"

CConnection::CConnection()
{
}

CConnection::~CConnection()
{
}

bool CConnection::send(const char* data, int size)
{
	return true;
}

void CConnection::close_connection()
{
}

void CConnection::reconnect()
{
}

bool CConnection::is_connected()
{
	// Connect(remote_addr_, remote_port_);
	return true;
}

void CConnection::get_addr(struct sockaddr* remoteaddr)
{
	//socklen_t len = sizeof(sockaddr);
	//if(::getpeername(fd_, remoteaddr, &len))
	//{
	//	LOG_ERR("err: %s", p_socket_last_error());
	//}
}

void CConnection::set_session(ISession* session)
{
	session_ = session;
}

void CConnection::set_parser(IPacketParser* parser)
{
	packet_parser_ = parser;
}