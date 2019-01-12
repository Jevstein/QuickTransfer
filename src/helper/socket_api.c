/*
 *  socket_api.cpp 
 *  socket_api
 *
 *  Created by Jevstein on 2018/11/1 16:05.
 *  Copyright @ 2018year Jevstein. All rights reserved.
 *
 */
#include "prefixhead.h"
#include "socket_api.h"

YI_SOCKET socket_create(const char *addr, int port, int type /*= SOCK_STREAM*/, struct addrinfo** addrinfo_res /*= NULL*/)
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
	if (0 != getaddrinfo((strcmp(addr, "") == 0 ? NULL : addr), port_str, &hints, &res))
	{
		LOG_ERR("failed to getaddrinfo!");
		return -1;
	}

	//2.create socket
	YI_SOCKET sockfd = ::socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (sockfd < 0)
	{
		LOG_ERR("failed to create socket! sockfd: %d, err: %s", sockfd, strerror(errno));
		return -2;
	}

	if (addrinfo_res)
		*addrinfo_res = res;

	LOG_INF("create socket[%s:%d] success, sockfd=%d", addr, port, sockfd);

	return sockfd;
}

bool socket_bind(YI_SOCKET sockfd, struct addrinfo* addrinfo_res)
{
	int ret = ::bind(sockfd, addrinfo_res->ai_addr, addrinfo_res->ai_addrlen);
	if (0 != ret)
	{
		LOG_ERR("failed to bind[%s|%d]! ret=%d, err: %s"
			, inet_ntoa(((struct sockaddr_in *)addrinfo_res->ai_addr)->sin_addr)
			, ntohs(((struct sockaddr_in *)addrinfo_res->ai_addr)->sin_port)
			, ret, strerror(errno));

		return false;
	}

	LOG_INF("bind socket[%s:%d] success!"
		, inet_ntoa(((struct sockaddr_in *)addrinfo_res->ai_addr)->sin_addr)
		, ntohs(((struct sockaddr_in *)addrinfo_res->ai_addr)->sin_port));

	return true;
}

bool socket_listen(YI_SOCKET sockfd, int backlog/* = 128*/)
{
	if (0 != ::listen(sockfd, backlog))
	{
		LOG_ERR("failed to listen! err: %s", strerror(errno));
		return false;
	}

	LOG_INF("listen socket success!");

	return true;
}

YI_SOCKET socket_accept(YI_SOCKET sockfd, struct sockaddr_in6 *addr)
{
	socklen_t addrlen = sizeof(sockaddr_in6);
	memset(addr, 0, addrlen);

	int connfd = ::accept(sockfd, (sockaddr*)addr, &addrlen);
	if (connfd < 0)
	{
		LOG_ERR("failed to accept, sockfd=%d, err: %s", connfd, strerror(errno));
		return -1;
	}

	LOG_INF("accept client[%s:%d] success, sockfd=%d"
		, inet_ntoa(((struct sockaddr_in *)addr)->sin_addr)
		, ntohs(((struct sockaddr_in *)addr)->sin_port)
		, connfd);

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

bool socket_connect(YI_SOCKET sockfd, struct addrinfo* addrinfo_res)
{
	int ret = ::connect(sockfd, addrinfo_res->ai_addr, addrinfo_res->ai_addrlen);
	if (ret < 0)
	{
		LOG_ERR("failed to connect[%s:%d]! ret=%d, err: %s"
			, inet_ntoa(((struct sockaddr_in *)addrinfo_res->ai_addr)->sin_addr)
			, ntohs(((struct sockaddr_in *)addrinfo_res->ai_addr)->sin_port)
			, ret, strerror(errno));

		return false;
	}

	LOG_INF("connect server[%s:%d] success!"
		, inet_ntoa(((struct sockaddr_in *)addrinfo_res->ai_addr)->sin_addr)
		, ntohs(((struct sockaddr_in *)addrinfo_res->ai_addr)->sin_port));

	return true;
}

int socket_send(YI_SOCKET sockfd, const void *buf, size_t len)
{
	int ret = ::send(sockfd, buf, len, 0);
	if (ret < 0)
	{
		LOG_ERR("failed to send! ret=%d, err: %s", ret, strerror(errno));
	}

	return ret;
}

int socket_recv(YI_SOCKET sockfd, void *buf, size_t len)
{
	int ret = ::recv(sockfd, buf, len, 0);
	if (ret < 0)
	{
		LOG_ERR("failed to recv! ret=%d, err: %s", ret, strerror(errno));
	}

	return ret;
}

void socket_close(YI_SOCKET sockfd)
{
	::close(sockfd);

	LOG_INF("close sockfd=%d success!", sockfd);
}

bool set_reuse_port(YI_SOCKET sockfd)
{
	int on = 1;
	int ret = ::setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(int));
	if (ret < 0)
	{
		LOG_ERR("failed to reuse port!");
		return false;
	}
	LOG_INF("reuse port success!");

	return true;
}

bool set_nonblocking(YI_SOCKET sockfd)
{
	int on = 1;
	::setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, (char*)&on, sizeof(on));

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
		return false;
	}
	//#endif

	return true;
}

int get_addrinfo(const struct sockaddr *addr, char ipaddr[INET6_ADDRSTRLEN], int *port)
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
		return -1;
	}

	if (NULL == inet_ntop(addr->sa_family, numeric_addr, ipaddr, INET6_ADDRSTRLEN))
	{
		LOG_ERR("failed to inet_ntop!");
		return -2;
	}

	return 0;
}
