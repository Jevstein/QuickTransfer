#ifndef _DELEGATE_H_
#define _DELEGATE_H_
#include "config.h"
#include "sessioncreator.h"

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
	CSessionCreator* get_session_creator() { return &session_creator_; }

private:
	bool started_;

	CConfig cfg_;

    CModuleFactory net_fac_;
    INetModule* net_module_;
	IListener* tcp_listener_;
	CSessionCreator session_creator_;
};

#define DELEGATE__ CDelegate::GetInstancePtr()
#define SESSION_CREATOR__ CDelegate::GetInstancePtr()->get_session_creator()

#endif //_DELEGATE_H_