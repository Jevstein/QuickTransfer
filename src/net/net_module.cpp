#include "prefixhead.h"
#include "net_module.h"

CNetModule* net_module__ = NULL;

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

}

void CNetModule::release()
{
	net_module__ = NULL;
	delete this;
}

int CNetModule::run(int limit /*= 1*/)
{
    LOG_INF("run...");
    return 0;
}

IListener* CNetModule::create_listener()
{
    LOG_INF("create_listener...");
    return 0;
}

IConnector* CNetModule::create_connector()
{

    LOG_INF("create_connector...");
    return 0;
}

void CNetModule::init()
{

    LOG_INF("CNetModule::init()...");
}