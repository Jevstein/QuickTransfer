#ifndef _DELEGATE_H_
#define _DELEGATE_H_
#include "config.h"

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

private:

	bool started_;

	CConfig cfg_;

    CModuleFactory net_fac_;
    INetModule* net_module_;
};

#define DELEGATE__ CDelegate::GetInstancePtr()

#endif //_DELEGATE_H_