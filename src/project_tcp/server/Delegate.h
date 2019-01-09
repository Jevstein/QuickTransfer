#ifndef _DELEGATE_H_
#define _DELEGATE_H_
//#include "UserSession.h"
//#include "AuthenSession.h"
//#include "Timer.h"
#include "Config.h"
#include "Launcher.h"

class CPlayer;

class CDelegate
{
	CDelegate();
public:
	SINGLETON(CDelegate);
	~CDelegate();

public:
	bool init();
	void run();
	void stop();

public:
	void log(const char* msg, ...);

	CConfig* get_cfg() { return &cfg_; }

	IBDConnector* create_connector() { return netmodule_->CreateConnector(); }

	bool connect_assever(const char *ip, int port, CPlayer *player);
	bool connect_dssever(const char *ip, int port, CPlayer *player);

private:
	modulefactory netfac_;		//net
	IBDNetModule* netmodule_;	//net module
	modulefactory logfac_;	//log
	IBDLogger* logger_;			//log object
	//IBDListener* _listener;	//listener
	//IBDConnector *connector_;	//connector
	std::vector<IBDConnector *> vct_of_authenconnector_;//AuthenServer connector
	//std::vector<IBDConnector *> _vct_of_userconnector;//CUseSession connector

	bool started_;

	CConfig cfg_;
	CLauncher launcher_;
};

#define DELEGATE__ CDelegate::GetInstancePtr()
#define CONFIG__ CDelegate::GetInstancePtr()->get_cfg()

#endif //_DELEGATE_H_