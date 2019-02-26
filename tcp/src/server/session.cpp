#include "prefixhead.h"
#include "session.h"
#include "delegate.h"

CSession::CSession()
: connection_(NULL)
, port_(0)
{
}

CSession::~CSession()
{
}

void CSession::release()
{
    LOG_INF("CSession::release");
    SESSION_CREATOR__->destroy(this);
}

void CSession::on_connection(IConnection* connection)
{
    LOG_INF("CSession::on_connection");

    if (connection)
    {
		struct sockaddr peer_addr;
		connection->get_addr(&peer_addr);
		addr_ = std::string((const char*)inet_ntoa(((sockaddr_in*)&peer_addr)->sin_addr));
		port_ = ntohs(((struct sockaddr_in *)&peer_addr)->sin_port);

		LOG_INF("successfully connected to server[%s:%d]", addr_.c_str(), port_);
        
        char msg[]="hello~ Bob, I'm Alice from server!";
        connection->send(msg, sizeof(msg));
	}

    connection_ = connection;
}

void CSession::on_disconnect()
{
    LOG_INF("CSession::on_disconnect");
}

void CSession::on_disconnection()
{
    LOG_INF("CSession::on_disconnection");
}

void CSession::on_recv(const IPacket *packet)
{
    LOG_INF("CSession::on_recv('%s', %d)", packet->data(), packet->length());
}
