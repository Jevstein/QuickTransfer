#ifndef __NET_SOCKET_H__
#define __NET_SOCKET_H__
#include "../helper/socket_api.h"

enum CONNTYPE
{
    SO_NULL = 0,
    SO_LISTENER,
    SO_CONNECTION,
    SO_CONNECTOR,
    SO_UDPCONNECTION,
    SO_UDPLISTENNER,
};

class INetSocket
{
public:
    INetSocket();
    virtual ~INetSocket();
    virtual void on_accept() = 0;
    virtual void on_recv() = 0;
    virtual void on_send() = 0;
    virtual void on_error() = 0;

public:
    int get_socket_type() const { return socket_type_; }

protected:
    int socket_type_;//CONNTYPE
    YI_SOCKET sock_fd_;
    struct addrinfo* addrinfo_res_;
};


#endif//__NET_SOCKET_H__