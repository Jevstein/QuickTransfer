/*
 *  NetTCPConnection.h 
 *  NetTCPConnection
 *
 *  Created by Jevstein on 2018/4/27 19:36.
 *  Copyright @ 2018year Jevstein. All rights reserved.
 *
 */
#ifndef _CONNECTION_H_
#define _CONNECTION_H_

class CConnection : public IConnection
{
public:
	CConnection();
    virtual ~CConnection();
    virtual bool send(const char* data, int size);
    virtual void close_connection();
    virtual void reconnect();
    virtual bool is_connected();
    virtual void get_addr(struct sockaddr* remoteaddr);
    virtual void set_session(ISession* session);

	// CConnection
    virtual void set_parser(IPacketParser* parser);

protected:
	ISession* session_;
	IPacketParser* packet_parser_;
};

#endif//_CONNECTION_H_

