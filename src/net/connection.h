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
#include "net_socket.h"

class CTCPEvent;
class CConnection : public IConnection, public INetSocket
{
public:
	enum
	{
		MAX_OVERLAP_BUFFER = 65535, /*MAX_PACKET_LEN * 2*/
		ON_CONNECT = -1,
		ON_CONNECTION = -2,
		ON_DISCONNECT = -3,
		ON_DISCONNECTION = -4,
		ON_CLOSECONNECTION = -5,
	};

    enum STATUS
    {
        UNCONNECT = 0,
        CONNECTING,
        CONNECTED,
    };

public:
	CConnection();
    virtual ~CConnection();
    virtual bool send(const char* data, int size);
    virtual void close_connection();
    virtual void reconnect();
    virtual bool is_connected() { return (CONNECTED == connect_status_); }
    virtual void get_addr(struct sockaddr* remoteaddr);
    virtual void set_session(ISession* session) { session_ = session; }
    
    // INetSocket
    virtual int socket_type() const { return SO_CONNECTION; }
	virtual void on_recv();
	virtual void on_send();
	virtual void on_accept() { assert(false); }
    virtual void on_error();

public:
    void set_parser(IPacketParser* parser) { packet_parser_ = parser; }
	bool connect(const char* addr, int port);
	void on_connect();
    void on_connection();
	void on_disconnect();
	void on_disconnection();
	void on_closeconnection();

    void _on_connection(int sockfd);
    void _on_disconnect();
	void _on_disconnection();

public:
    ISession* get_session() { return session_; }

protected:
	void clear_send_queue();

protected:
	ISession* session_;
	IPacketParser* packet_parser_;

	CEventQueue<CTCPEvent*, CLocker> que_of_send_buffer_;
    char* recv_buffer_;//[MAX_OVERLAP_BUFFER];	--> // ring buffer
	int recv_offset_;

private:
    bool is_client_;
    int connect_status_;

	char remote_addr_[64];
	int remote_port_;
};

#endif//_CONNECTION_H_

