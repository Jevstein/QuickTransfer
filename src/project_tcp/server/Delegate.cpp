#include "PrefixHead.h"
#include "Delegate.h"
#include "PlayerManager.h"

CDelegate::CDelegate()
: netfac_("libbd_netmodule.so")
, logfac_("libbd_logger.so")
, netmodule_(NULL)
, logger_(NULL)
, started_(true)
{
}

CDelegate::~CDelegate()
{
	SAFE_RELEASE(netmodule_);
	SAFE_RELEASE(logger_);

	// 3.AuthenSession connector
	for (size_t i = 0; i < vct_of_authenconnector_.size(); i++)
	{
		SAFE_RELEASE(vct_of_authenconnector_.at(i));
	}
	vct_of_authenconnector_.clear();

	cfg_.cleanup();
}

bool CDelegate::init()
{
	// 1.log module
	logger_ = logfac_.createmodule<IBDLogger>("CreateLogger");
	if (logger_ == 0 || !logger_->Init(0, "DirServerDemo_log", 0))
	{
		LOG_ERR("failed to load log module!");
		return false;
	}

	 // 2.load libbd_netmodule.so
	netmodule_ = netfac_.createmodule<IBDNetModule>("CreateNetModule");
	if (netmodule_ == 0)
	{
		LOG_ERR("failed to load libbd_netmodule.so!");
		return false;
	}

	// 3.load xml
	if (!cfg_.init("DirServerDemo.xml"))
	{
		LOG_ERR("failed to load DirServerDemo.xml!");
		return false;
	}

	//// 4.create connector
	//if (!create_connector())
	//{
	//	LOG_ERR("Failed to create server!");
	//	return false;
	//}

	return true;
}

void CDelegate::run()
{
	//started_ = prepare_connect();
	//if (!started_)
	//{
	//	LOG_ERR("Failed to connect server!");
	//	return;
	//}

	started_ = true;

	while (started_)
	{
		bool idle = true;

		if (netmodule_ && netmodule_->Run())
			idle = false;

		launcher_.run();

		if (idle)
			usleep(1000);
	}
}

void CDelegate::stop()
{
	started_ = false;

}

void CDelegate::log(const char* msg, ...)
{
	//// 无日志
	//if (CONFIG__->get_log_mode() == 0)
	//	return;

	char buffer[2048];
	memset(buffer, 0, 2048);

	va_list argptr;
	try
	{
		va_start(argptr, msg);
		vsprintf(buffer, msg, argptr);
		va_end(argptr);

		//// 输出到屏幕
		//if (CONFIG__->get_log_mode() == 1)
			glog.log(buffer);

		//// 输出到文件
		//if (CONFIG__->get_log_mode() == 2 && logger_)
		//	logger_->Log(buffer);
	}
	catch (...)
	{
		//
	}
}

//void CDelegate::force_log(const char* msg, ...)
//{
//	char buffer[2048];
//	memset(buffer, 0, 2048);
//
//	va_list argptr;
//	try
//	{
//		va_start(argptr, msg);
//		vsprintf(buffer, msg, argptr);
//		va_end(argptr);
//
//		// 输出到屏幕
//		glog.log(buffer);
//
//		// 输出到文件
//		if (logger_)
//			logger_->Log(buffer);
//	}
//	catch (...)
//	{
//		//
//	}
//}

//bool CDelegate::connect_ds_server(const char *ip, int port, CPlayer *player)
//{
//	CDirSession *session = player->get_dirsession();
//	if (!session)
//	{
//		LOG_ERR("Failed to connect ds server, for session is null!");
//		return false;
//	}
//
//	IBDConnector *connector = session->get_connector();
//	if (!connector)
//	{
//		LOG_ERR("Failed to connect ds server, for connector is null!");
//		return false;
//	}
//
//	connector->SetSession(session);
//	connector->Connect(CONFIG__->ds_listen_ip(), CONFIG__->ds_listen_port());
//
//	return true;
//}

bool CDelegate::connect_assever(const char *ip, int port, CPlayer *player)
{
	IBDConnector *connector = netmodule_->CreateConnector();
	if (!connector)
		return false;

	connector->SetSession(player->get_authensession());
	connector->Connect(ip, port);

	vct_of_authenconnector_.push_back(connector);

	return true;
}
