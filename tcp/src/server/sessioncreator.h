/***************************************************************************** 
 *  @COPYRIGHT NOTICE 
 *  @Copyright (c) 2017, Microbeam software technology (Shanghai) co. LTD 
 *  @All rights reserved 
 
 *  @file     : sessioncreator.h 
 *  @version  : ver 1.0 
 
 *  @author   : yiwenqiang 
 *  @date     : 2019/01/12 10:20 
 *  @brief    : brief 
*****************************************************************************/ 
#ifndef _SESSION_CREATOR_H_
#define _SESSION_CREATOR_H_

class CSessionCreator : public ISessionCreator
{
public:
    CSessionCreator();
    virtual ~CSessionCreator();
    virtual bool on_preaccept(struct sockaddr* remoteaddr);
    virtual ISession* on_create();
    virtual void destroy(ISession* session);
};

#endif //_SESSION_CREATOR_H_