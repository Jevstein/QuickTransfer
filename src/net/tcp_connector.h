/*
 *  NetTCPConnector.h 
 *  NetTCPConnector
 *
 *  Created by Jevstein on 2018/4/20 19:56.
 *  Copyright @ 2018year Jevstein. All rights reserved.
 *
 */
#ifndef _TCP_CONNECTOR_H_
#define _TCP_CONNECTOR_H_

class CConnection;
class CTCPConnector : public IConnector
{
public:
	CTCPConnector();
    virtual ~CTCPConnector();
    virtual void release();
    virtual bool connect(const char* remote_addr, int port);
    virtual void set_session(ISession* session);
    virtual void set_parser(IPacketParser* parser);

private:
	CConnection* connection_;
};

#endif//_TCP_CONNECTOR_H_
