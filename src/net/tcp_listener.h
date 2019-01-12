/*
 *  NetTCPListener.h 
 *  NetTCPListener
 *
 *  Created by Jevstein on 2018/4/20 17:42.
 *  Copyright @ 2018year Jevstein. All rights reserved.
 *
 */
#ifndef TCP_LISTENER_H_
#define TCP_LISTENER_H_

#include "connection.h"
#include "packet_parser.h"

class CTCPListener : public IListener
{
public:
	CTCPListener(void);
	virtual ~CTCPListener(void);
    virtual void release();
    virtual bool start_listen(const char* listen_addr, int port);
    virtual void stop_listen();
    virtual void set_parser(IPacketParser* parser);
    virtual void set_creator(ISessionCreator* creator);

	// CTCPListener
	virtual void on_accept();

public:
	inline ISessionCreator* GetSessionCreator() const { return session_creator_; }

private:
	IPacketParser* packet_parser_;
	ISessionCreator* session_creator_;
};

#endif//TCP_LISTENER_H_
