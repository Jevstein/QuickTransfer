#include "prefixhead.h"
#include "tcp_connector.h"
#include "connection.h"

CTCPConnector::CTCPConnector()
: connection_(NULL)
{
	//LOG_DBG(" CTCPConnector");

    // connection_ = NET_POOL->PopNetTCPConnection();
}

 CTCPConnector::~ CTCPConnector(void)
{
	//LOG_DBG("~ CTCPConnector");

	connection_->set_parser(NULL);
	connection_->set_session(NULL);
	// connection_->on_closeconnection();
	// NET_POOL->PushNetTCPConnection((CNetTCPConnection *)connection_);
}

void CTCPConnector::release()
{
	delete this;
}

bool CTCPConnector::connect(const char* remote_addr, int port)
{
	if (!connection_)
	{
		LOG_ERR("the connection_ is null!");
		return false;
	}

	// return connection_->connect(remote_addr, port);
	return true;
}

void CTCPConnector::set_session(ISession* session)
{
	connection_->set_session(session);
}

void CTCPConnector::set_parser(IPacketParser* parser)
{
	connection_->set_parser(parser);
}
