#include "prefixhead.h"
#include "tcp_connector.h"
// #include "connection.h"
#include "net_module.h"

CTCPConnector::CTCPConnector()
: connection_(NULL)
{
	//LOG_DBG(" CTCPConnector");
}

CTCPConnector::~ CTCPConnector(void)
{
	if (connection_)
	{
		connection_->set_parser(NULL);
		connection_->set_session(NULL);
		// connection_->on_closeconnection();
		get_module()->get_pool()->push_connection(connection_);
		connection_ = NULL;
	}
}

void CTCPConnector::release()
{
	delete this;
}

bool CTCPConnector::connect(const char* addr, int port)
{
	if (!connection_)
	{
		connection_ = get_module()->get_pool()->pop_connection();
		if (!connection_)
		{
			LOG_ERR("the connection_ is null!");
			return false;
		}

		connection_->set_session(session_);
		connection_->set_parser(parser_);
	}

	return connection_->connect(addr, port);
}
