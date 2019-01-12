#include "prefixhead.h"
#include "delegate.h"

CDelegate::CDelegate()
: net_fac_("libnet.so")
, net_module_(NULL)
, started_(true)
{
}

CDelegate::~CDelegate()
{

}

bool CDelegate::init()
{
	cfg_.init();
	    
	net_module_ = net_fac_.createmodule<INetModule>("create_net_module");
    if (net_module_ == 0)
    {
        LOG_ERR("failed to create net module!");
        return false;
    }

// #if TCP_DEMO
// 	tcp_listener_ = net_module_->CreateListener();
// 	if (tcp_listener_ == 0)
// 	{
// 		LOG_ERR("failed to create tcp listener!");
// 		return false;
// 	}
// #endif//TCP_DEMO

// #if UDP_DEMO
// 	udp_listener_ = net_module_->CreateUdpListener();
// 	if (udp_listener_ == 0)
// 	{
// 		LOG_ERR("failed to create udp listener!");
// 		return false;
// 	}
// #endif//UDP_DEMO

// 	if (!prepare_listen())
// 	{
// 		LOG_ERR("failed to prepare to listen!");
// 		return false;
// 	}


	return true;
}

void CDelegate::run()
{

}

void CDelegate::stop()
{
	started_ = false;
}
