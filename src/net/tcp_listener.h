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
#include "net_socket.h"
#include "packet_parser.h"

class CTCPListener : public IListener, public INetSocket
{
public:
	CTCPListener(void);
	virtual ~CTCPListener(void);
    virtual void release();
    virtual bool start_listen(const char* listen_addr, int port);
    virtual void stop_listen();
    virtual void set_parser(IPacketParser* parser) { packet_parser_ = parser; }
    virtual void set_creator(ISessionCreator* creator) { session_creator_ = creator; }

	// INetIOEvent
    virtual void on_accept();
    virtual void on_recv() {}
    virtual void on_send() {}
	virtual void on_error() {}

private:
	IPacketParser* packet_parser_;
	ISessionCreator* session_creator_;
};

#endif//TCP_LISTENER_H_
