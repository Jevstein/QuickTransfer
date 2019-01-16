#include "prefixhead.h"
#include "delegate.h"

CDelegate::CDelegate()
: net_fac_("./libnet.so")
, net_module_(NULL)
, tcp_connector_(NULL)
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
    if (net_module_ == NULL)
    {
        LOG_ERR("failed to create net module!");
        return false;
    }

	tcp_connector_ = net_module_->create_connector();
	if (tcp_connector_ == NULL)
	{
		LOG_ERR("failed to create tcp connector!");
		return false;
	}
	tcp_connector_ ->set_session(&session_);

	std::string tcp_ip = cfg_.get_cfg_info()->ip;
	int tcp_port = cfg_.get_cfg_info()->port;
	if (!tcp_connector_->connect(tcp_ip.c_str(), tcp_port))
	{
		LOG_ERR("failed to listen[%s.%d]!", tcp_ip.c_str(), tcp_port);
		return false;
	}

	return true;
}

void CDelegate::run()
{
    while (started_)
    {
        bool idle = true;
        
        if (net_module_->run() == 0)
            idle = false;
        
        if (idle)
            p_sleep(100);
    }
}

void CDelegate::stop()
{
	started_ = false;
}
