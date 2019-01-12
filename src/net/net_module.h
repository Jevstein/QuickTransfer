#ifndef __NET_MODULE_H__
#define __NET_MODULE_H__

class CNetModule : public INetModule 
{
public:
    CNetModule();
    virtual ~CNetModule();
	virtual void release();
	virtual int run(int limit = 1);
	virtual IListener* create_listener();
	virtual IConnector* create_connector();

protected:
    void init();
};

extern "C" /*P_EXPORT_DLL*/ INetModule* create_net_module();

#endif//__NET_MODULE_H__