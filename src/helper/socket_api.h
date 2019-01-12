/*
 *  socket_api.h 
 *  socket_api
 *
 *  Created by Jevstein on 2018/11/1 10:20.
 *  Copyright @ 2018year Jevstein. All rights reserved.
 *
 */

#ifndef __SOCKET_API_H__
#define __SOCKET_API_H__

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
//#include <netinet/tcp.h> 

typedef int YI_SOCKET;

/*create socket*/
YI_SOCKET socket_create(const char *addr, int port, int type = SOCK_STREAM, struct addrinfo** addrinfo_res = NULL);
/*bind*/
bool socket_bind(YI_SOCKET sockfd, struct addrinfo* addrinfo_res);
/*listen*/
bool socket_listen(YI_SOCKET sockfd, int backlog = 128);
/*accept*/
YI_SOCKET socket_accept(YI_SOCKET sockfd, struct sockaddr_in6 *addr);
/*connect*/
bool socket_connect(YI_SOCKET sockfd, struct addrinfo* addrinfo_res);
/*send*/
int socket_send(YI_SOCKET sockfd, const void *buf, size_t len);
/*recv*/
int socket_recv(YI_SOCKET sockfd, void *buf, size_t len);
/*close*/
void socket_close(YI_SOCKET sockfd);

/*reuse socket*/
bool set_reuse_port(YI_SOCKET sockfd);
/*nonblocking*/
bool set_nonblocking(YI_SOCKET sockfd);
/*get addr info*/
int get_addrinfo(const struct sockaddr *addr, char ipaddr[INET6_ADDRSTRLEN], int *port);

#endif //__SOCKET_API_H__