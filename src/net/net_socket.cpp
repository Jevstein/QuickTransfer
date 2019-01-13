#include "prefixhead.h"
#include "net_socket.h"

INetSocket::INetSocket() 
: socket_type_(SO_NULL)
, sock_fd_(-1)
, addrinfo_res_(NULL)
{

}

INetSocket::~INetSocket()
{

}