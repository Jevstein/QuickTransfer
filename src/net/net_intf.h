#ifndef __NET_INTF_H__
#define __NET_INTF_H__

class ISession;

class IPacket
{
public:
    virtual ~IPacket(){}
    virtual int id() const = 0;
    virtual const char* data() const = 0; 
    virtual int length() const = 0;
};

class IConnection
{
public:
    virtual ~IConnection(){}
    virtual bool send(const IPacket *packet) = 0;
    virtual bool send(const char *data, int len) = 0;
    virtual void close_connection() = 0;
    virtual void reconnect() = 0;
    virtual bool is_connected() = 0;
    virtual void get_addr(struct sockaddr* remote_addr) = 0;
    virtual void set_session(ISession* session) = 0;
};

class ISession
{
public:
    virtual ~ISession() {}
    virtual void release() = 0;
    virtual void on_connection(IConnection* connection) = 0;
    virtual void on_disconnect() = 0;
    virtual void on_disconnection() = 0;
    virtual void on_recv(const IPacket *packet) = 0;
};

class ISessionCreator
{
public:
    virtual ~ISessionCreator() {}
    virtual bool on_preaccept(struct sockaddr* remote_addr) = 0;
    virtual ISession* on_create() = 0;
    virtual void destroy(ISession* session) = 0;
};

class IPacketParser
{
public:
    virtual ~IPacketParser() {}
    virtual int encode(const IPacket* packet, char* out_data, int& out_size) = 0;
    virtual int decode(IPacket *packet, int& max_len, const char* data, int size) = 0;
	virtual int encode(const char* in_data, int in_len, char* out_data, int& out_len) = 0;
	virtual int decode(const char* in_data, int in_len, char* out_data, int& out_len) = 0;
};

class IListener
{
public:
    virtual ~IListener() {}
    virtual void release() = 0;
    virtual bool start_listen(const char* addr, int port) = 0;
    virtual void stop_listen() = 0;
    virtual void set_parser(IPacketParser* parser) = 0;
    virtual void set_creator(ISessionCreator* creator) = 0;
};

class IConnector
{
public:
    virtual ~IConnector() {}
    virtual void release() = 0;
    virtual bool connect(const char* addr, int port) = 0;
    virtual void set_session(ISession* session) = 0;
    virtual void set_parser(IPacketParser* parser) = 0;
};

class INetModule
{
public:
    virtual ~INetModule() {}
	virtual void release() = 0;
	virtual int run(int limit = 1) = 0;
	virtual IListener* create_listener() = 0;
	virtual IConnector* create_connector() = 0;
};

#endif//__NET_INTF_H__