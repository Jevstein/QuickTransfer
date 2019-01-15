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

bool INetSocket::socket_create(const char *addr, int port, int type /*= SOCK_STREAM*/)
{
	char port_str[16] = { 0 };
	sprintf(port_str, "%d", port);

	//1.initialize address
	struct addrinfo hints;
	struct addrinfo* res = NULL;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = type;		//TCP=SOCK_STREAM, UDP=SOCK_DGRAM
	hints.ai_protocol = IPPROTO_IP;	//IPPROTO_TCP
	hints.ai_flags = AI_PASSIVE;	//AI_CANONNAME
	if (0 != ::getaddrinfo((strcmp(addr, "") == 0 ? NULL : addr), port_str, &hints, &res))
	{
		LOG_ERR("failed to getaddrinfo!");
		return false;
	}

	//2.create socket
	sock_fd_ = ::socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (sock_fd_ < 0)
	{
		LOG_ERR("failed to create socket! sockfd: %d, err: %s", sock_fd_, strerror(errno));
		return false;
	}
    
    addrinfo_res_ = res;

	return true;
}

bool INetSocket::socket_connect()
{
	int ret = ::connect(sock_fd_, addrinfo_res_->ai_addr, addrinfo_res_->ai_addrlen);
	if (ret < 0)
	{
		LOG_ERR("failed to connect[%s:%d]! ret=%d, err: %s"
			, inet_ntoa(((struct sockaddr_in *)addrinfo_res_->ai_addr)->sin_addr)
			, ntohs(((struct sockaddr_in *)addrinfo_res_->ai_addr)->sin_port)
			, ret, strerror(errno));

		return false;
	}

	return true;
}

bool INetSocket::socket_bind()
{
	int ret = ::bind(sock_fd_, addrinfo_res_->ai_addr, addrinfo_res_->ai_addrlen);
	if (0 != ret)
	{
		LOG_ERR("failed to bind[%s|%d]! ret=%d, err: %s"
			, inet_ntoa(((struct sockaddr_in *)addrinfo_res_->ai_addr)->sin_addr)
			, ntohs(((struct sockaddr_in *)addrinfo_res_->ai_addr)->sin_port)
			, ret, strerror(errno));

		return false;
	}

	return true;
}

bool INetSocket::socket_listen(int backlog/* = 128*/)
{
	if (0 != ::listen(sock_fd_, backlog))
	{
		LOG_ERR("failed to listen! err: %s", strerror(errno));
		return false;
	}

	return true;
}

int INetSocket::socket_accept(struct sockaddr_in6 *addr)
{
	socklen_t addrlen = sizeof(sockaddr_in6);
	memset(addr, 0, addrlen);

	int connfd = ::accept(sock_fd_, (sockaddr*)addr, &addrlen);
	if (connfd < 0)
	{
		LOG_ERR("failed to accept, sockfd=%d, err: %s", connfd, strerror(errno));
	}

	//{
	//	char c_ipaddr[INET6_ADDRSTRLEN];
	//	int c_port = 0;
	//	if (0 == get_addrinfo((struct sockaddr*)addr, c_ipaddr, &c_port))
	//	{
	//		LOG_INF("accept client[%s:%d] success!", c_ipaddr, c_port);
	//	}
	//}

	return connfd;
}

int INetSocket::socket_send(const void *buf, size_t len)
{
	int ret = ::send(sock_fd_, buf, len, 0);
	if (ret < 0)
	{
		LOG_ERR("failed to send! ret=%d, err: %s", ret, strerror(errno));
	}

	return ret;
}

int INetSocket::socket_recv(void *buf, size_t len)
{
	int ret = ::recv(sock_fd_, buf, len, 0);
	if (ret < 0)
	{
		LOG_ERR("failed to recv! ret=%d, err: %s", ret, strerror(errno));
	}

	return ret;
}

void INetSocket::socket_close()
{
	CLOSE_SOCKET(sock_fd_);
    sock_fd_ = -1;
}

bool INetSocket::set_reuse_port()
{
	int on = 1;
	int ret = ::setsockopt(sock_fd_, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(int));
	if (ret < 0)
	{
		LOG_ERR("failed to reuse port!");
		return false;
	}

	return true;
}

bool INetSocket::set_nonblocking(int sockfd)
{
	// int on = 1;
	// ::setsockopt(sock_fd_, SOL_SOCKET, SO_KEEPALIVE, (char*)&on, sizeof(on));

	//#ifdef WIN32
	//	unsigned long ul = 1;
	//	int ret = ::ioctlsocket(sock, FIONBIO, &ul);
	//	if (ret == SOCKET_ERROR)
	//	{
	//		return false;
	//	}
	//
	//#else
	int opts = fcntl(sockfd, F_GETFL);
	opts = (opts < 0) ? O_NONBLOCK : opts | O_NONBLOCK;
	if (fcntl(sockfd, F_SETFL, opts) < 0)
	{
		LOG_ERR("failed to fcntl!");
		return false;
	}
	//#endif

	return true;
}

bool INetSocket::get_addrinfo(const struct sockaddr *addr, char ipaddr[INET6_ADDRSTRLEN], int *port)
{
	void *numeric_addr = NULL;
	//char addr_buff[INET6_ADDRSTRLEN];

	if (AF_INET == addr->sa_family)
	{
		numeric_addr = &((struct sockaddr_in*)addr)->sin_addr;
		*port = ntohs(((struct sockaddr_in*)addr)->sin_port);
	}
	else if (AF_INET6 == addr->sa_family)
	{
		numeric_addr = &((struct sockaddr_in6*)addr)->sin6_addr;
		*port = ntohs(((struct sockaddr_in6*)addr)->sin6_port);
	}
	else
	{
		LOG_ERR("sa_family[%d] is illegal!", addr->sa_family);
		return false;
	}

	if (NULL == inet_ntop(addr->sa_family, numeric_addr, ipaddr, INET6_ADDRSTRLEN))
	{
		LOG_ERR("failed to inet_ntop!");
		return false;
	}

	return true;
}
