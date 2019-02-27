#include "inner.h"
#include "dn_socket.h"
#include "udp-piece.h"

#define MAX_MSG_SIZE (256*1024)              /*recv缓冲器大小*/

static void recv_data_loop(void *user_data)
{
    dn_socket_t *dn_socket = (dn_socket_t *)user_data;
    int sockfd = dn_socket->sockfd;

    /*客户机的地址信息及长度信息*/
    struct sockaddr_in client;
    socklen_t client_len;
    unsigned char recvbuf[MAX_MSG_SIZE];
    int len = 0;
    char client_ip[64] = {0};

    // 分片
    udp_piece_t *udp_piece;
	udp_piece = udp_piece_init(64 * 1024);

    while(!dn_socket->exit_thread)
    {
        len = ::recvfrom(sockfd, recvbuf, sizeof(recvbuf), 0, (struct sockaddr *)&client, &client_len);
        if(len < 0)
        {
            LOG_ERR("recvfrom error!");
            return;
        }

        if(len > 0)  
        {  
            /*打印客户端地址和端口号*/
            inet_ntop(AF_INET, &client.sin_addr, client_ip, sizeof(client_ip));
            LOG_INF("sockfd=%d, client [%s:%d]", sockfd, client_ip, ntohs(client.sin_port));

			int ret = udp_piece_merge_ex(udp_piece, recvbuf, len);
			if(ret == 1)
			{
				LOG_INF("received：msg='%s', len=%d",udp_piece->recv_buf, udp_piece->recv_len);
				udp_piece_reset(udp_piece);
			}
			else if(ret == -1)
			{
				LOG_ERR("exception of udp piece!");
				udp_piece_reset(udp_piece);
			}
			else if(ret == 0)
			{
				LOG_INF("recv: ret=%d, len=%d", ret, len);
			}
			else
			{
				LOG_INF("unknown exception of udp piece!");
				udp_piece_reset(udp_piece);
			}
        }
    }
}

int dn_socket_init(dn_socket_t *dn_socket)
{
    dn_socket->sockfd = ::socket(AF_INET, SOCK_DGRAM, 0);
    if (dn_socket->sockfd < 0)
        return -1;

    bzero(&dn_socket->servaddr, sizeof(dn_socket->servaddr));
    dn_socket->servaddr.sin_family = AF_INET;
    dn_socket->servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    dn_socket->servaddr.sin_port = htons(dn_socket->srv_port);
   // setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(timeval))；

    // printf("Accepting connections[%d] ...\n", dn_socket->srv_port);

    if (::bind(dn_socket->sockfd, (struct sockaddr *)&dn_socket->servaddr, sizeof(dn_socket->servaddr)) < 0)
        return -2;

    return 0;
}

void dn_socket_uninit(dn_socket_t *dn_socket)
{
    if (!dn_socket)
        return;

    ::close(dn_socket->sockfd);
}

int dn_socket_loop(dn_socket_t *dn_socket)
{
    assert(dn_socket);

    dn_socket->exit_thread = 0;
    recv_data_loop(dn_socket);
    return 0;
}

int dn_socket_send_data(dn_socket_t *dn_socket, const void *buf, int size)
{
    if (::sendto(dn_socket->sockfd, buf, size, 0, (struct sockaddr*)&dn_socket->cliaddr, dn_socket->cliaddr_len) < 0)
    {
        printf("sendto error\n");
        exit(1);
    }

    return 0;
}
