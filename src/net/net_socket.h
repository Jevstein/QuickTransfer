#ifndef __NET_SOCKET_H__
#define __NET_SOCKET_H__
#include <stdio.h>
#include <stdlib.h>
#include <time.h>			//time(NULL)
#include <string.h>			//memset
#include <unistd.h>			//fcntl
#include <fcntl.h>			//fcntl
#include <sys/types.h>		//socket
#include <sys/socket.h>		//socket
#include <netdb.h>			//ipv6: addrinfo
#include <errno.h>			//strerror(errno)
#include <netinet/in.h>		//inet_ntoa
#include <arpa/inet.h>		//inet_ntoa
// #include <netinet/tcp.h> 

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
    virtual int socket_type() const = 0;
    virtual void on_accept() = 0;
    virtual void on_recv() = 0;
    virtual void on_send() = 0;
    virtual void on_error() = 0;

public:
    bool socket_create(const char *addr, int port, int type = SOCK_STREAM);
    bool socket_connect();
    bool socket_bind();
    bool socket_listen(int backlog = 128);
    int socket_accept(struct sockaddr_in6 *addr);
    int socket_send(const void *buf, size_t len);
    int socket_recv(void *buf, size_t len);
    void socket_close();

    bool set_reuse_port();
    static bool set_nonblocking(int sockfd);
    static bool get_addrinfo(const struct sockaddr *addr, char ipaddr[INET6_ADDRSTRLEN], int *port);

    int get_sockid() const { return sock_fd_; }

protected:
    int sock_fd_;
    struct addrinfo* addrinfo_res_;
};


#endif//__NET_SOCKET_H__