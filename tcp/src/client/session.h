/***************************************************************************** 
 *  @COPYRIGHT NOTICE 
 *  @Copyright (c) 2017, Microbeam software technology (Shanghai) co. LTD 
 *  @All rights reserved 
 
 *  @file     : session.h 
 *  @version  : ver 1.0 
 
 *  @author   : yiwenqiang 
 *  @date     : 2019/01/12 10:20 
 *  @brief    : brief 
*****************************************************************************/ 
#ifndef _SESSION_H_
#define _SESSION_H_
#include "../net/net_intf.h"

class CSession : public ISession
{
public:
	CSession();
    virtual ~CSession();
    virtual void release();
    virtual void on_connection(IConnection* connection);
    virtual void on_disconnect();
    virtual void on_disconnection();
    virtual void on_recv(const IPacket *packet);

private:
	IConnection* connection_;

	std::string addr_;
	short port_;
};

#endif //_SESSION_H_