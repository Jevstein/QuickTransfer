#include "prefixhead.h"
#include "net_module.h"
#include "tcp_listener.h"
#include "tcp_connector.h"

CNetModule* net_module__ = NULL;

CNetModule* get_module()
{
    return net_module__;
}

extern "C" /*P_EXPORT_DLL*/ INetModule* create_net_module()
{
	if (!net_module__)
	{
		net_module__ = new CNetModule();
	}

	return net_module__;
}

CNetModule::CNetModule()
{
    init();
}

CNetModule::~CNetModule()
{
    reactor_.misfire();
}

void CNetModule::release()
{
	net_module__ = NULL;
	delete this;
}

int CNetModule::run(int limit /*= 1*/)
{
	IEvent* ev = main_event_queue_.pop();
	if (ev == NULL)
	{
		return 1;
	}

	ev->process();
	ev->release();

    return 0;
}

IListener* CNetModule::create_listener()
{
    return new CTCPListener();
}

IConnector* CNetModule::create_connector()
{
    return new CTCPConnector();
}

void CNetModule::init()
{
    LOG_INF("CNetModule::init()...");

	reactor_.fire();
}