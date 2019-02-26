#ifndef __NET_MODULE_H__
#define __NET_MODULE_H__
#include "connection.h"
#include "reactor.h"
#include "net_memory.h"
#include "net_event.h"
#include "main_event_queue.h"

/** CNetObjPool*/
class CNetObjPool
{
public:
    CNetObjPool() {}
    ~CNetObjPool() {}
    
public:
    // tcp
    CTCPEvent* pop_tcpvent(bool bPriv = false) { return new CTCPEvent(); }
    void push_tcpevent(CTCPEvent* p) { if (p) delete p; }
    
    CConnection* pop_connection() { return new CConnection(); }
    void push_connection(CConnection* p) { if (p) delete p; }
  
private:
    // CObjectPool<CConnection, MAX_PLAYER_CAPABILITY, CLocker> pool_of_tcp_connection_;
 };

class CNetModule : public INetModule 
{
public:
    CNetModule();
    virtual ~CNetModule();
	virtual void release();
	virtual int run(int limit = 1);
	virtual IListener* create_listener();
	virtual IConnector* create_connector();

public:
	CReactor* get_reactor() { return &reactor_; }
	CNetObjPool* get_pool() { return &obj_pool_; }
	CNetMemory* get_memory() { return &net_memory_; }
	CMainEventQueue* main_event_queue() { return &main_event_queue_; }

protected:
    void init();

private:
	CReactor reactor_; 
	CNetObjPool obj_pool_;
	CNetMemory net_memory_;
	CMainEventQueue main_event_queue_;
};

CNetModule* get_module();
extern "C" /*P_EXPORT_DLL*/ INetModule* create_net_module();

#endif//__NET_MODULE_H__