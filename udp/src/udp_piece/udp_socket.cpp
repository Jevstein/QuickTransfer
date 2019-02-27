#include "inner.h"
#include "udp_socket.h"
#include "udp_piece.h"

#define MAX_MSG_SIZE (256 * 1024) /*recv缓冲器大小*/

#define RECV_CALLBACK(s, t, d, l) {         \
    udp_socket_result_t r;                  \
    r.type = t;                             \
    r.data = d;                             \
    r.len = l;                              \
    s->recv_func_callback(s->user_data, &r);\
}

static void _on_recv_data_loop(void *user_data)
{
    udp_socket_t *udp_socket = (udp_socket_t *)user_data;
    assert(udp_socket || udp_socket->recv_func_callback);
    udp_socketinfo_t *info = &udp_socket->srv_info;

    /*客户机的地址信息及长度信息*/
    char client_ip[64] = {0};
    struct sockaddr_in client;
    socklen_t client_len = sizeof(struct sockaddr);//此处若不赋值，首次recvfrom的ip地址总会为: 0.0.0.0

    unsigned char recvbuf[MAX_MSG_SIZE];
    int len = 0;

    // 分片
    udp_piece_t *udp_piece = udp_piece_init(64 * 1024);

    while(!udp_socket->exit_thread)
    {
        len = ::recvfrom(info->sockfd, recvbuf, sizeof(recvbuf), 0, (struct sockaddr *)&client, &client_len);
        if(len < 0)
        {
            // LOG_ERR("recvfrom error!");
            RECV_CALLBACK(udp_socket, RECV_TYPE_ERROR, recvbuf, len);
            return;
        } 
        else if(len > 0) 
        {  
            /*打印客户端地址和端口号*/
            inet_ntop(AF_INET, &client.sin_addr, client_ip, sizeof(client_ip));
            LOG_INF("sockfd=%d, client [%s:%d]", info->sockfd, client_ip, ntohs(client.sin_port));

			int ret = udp_piece_merge_ex(udp_piece, recvbuf, len);
			if(ret == 1)
			{
				// LOG_INF("received：msg='%s', len=%d", udp_piece->recv_buf, udp_piece->recv_len);
                RECV_CALLBACK(udp_socket, RECV_TYPE_SUCCESS, udp_piece->recv_buf, udp_piece->recv_len);
			}
			else if(ret == -1)
			{
				// LOG_ERR("exception of udp piece!");
                RECV_CALLBACK(udp_socket, RECV_TYPE_EXCEPTION, recvbuf, len);
			}
			else if(ret == 0)
			{
				// LOG_INF("recv: ret=%d, len=%d", ret, len);
                RECV_CALLBACK(udp_socket, RECV_TYPE_INCOMPLETE, recvbuf, len);
                continue;
			}
			else
			{
				// LOG_INF("unknown exception of udp piece!");
                RECV_CALLBACK(udp_socket, RECV_TYPE_UNKNOWN, recvbuf, len);
			}

			udp_piece_reset(udp_piece);
        } 
        else
        {
			// LOG_ERR("recvfrom: len=%d", len);
            RECV_CALLBACK(udp_socket, RECV_TYPE_UNKNOWN, recvbuf, len);
        }
    }
}

int udp_socket_init(udp_socket_t *udp_socket)
{
    assert(udp_socket);

    udp_socketinfo_t *info = &udp_socket->srv_info;

    info->sockfd = ::socket(AF_INET, SOCK_DGRAM, 0);
    if (info->sockfd < 0)
        return -1;

    bzero(&info->addr, sizeof(info->addr));
    info->addr.sin_family = AF_INET;
    info->addr.sin_addr.s_addr = htonl(INADDR_ANY);
    info->addr.sin_port = htons(info->port);
   // setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(timeval))；

    // printf("Accepting connections[%d] ...\n", info->port);

    return 0;
}

void udp_socket_uninit(udp_socket_t *udp_socket)
{
    if (!udp_socket)
        return;

    udp_socketinfo_t *info = &udp_socket->srv_info;

    ::close(info->sockfd);
}

int udp_socket_loop(udp_socket_t *udp_socket)
{
    assert(udp_socket);

    udp_socket->exit_thread = 0;
    _on_recv_data_loop(udp_socket);
    return 0;
}

int udp_socket_bind(udp_socket_t *udp_socket)
{
    udp_socketinfo_t *info = &udp_socket->srv_info;

    if (::bind(info->sockfd, (struct sockaddr *)&info->addr, sizeof(info->addr)) < 0)
        return -2;

    return 0;
}

int udp_socket_send(udp_socket_t *udp_socket, const void *buf, int len)
{
    assert(udp_socket);

    udp_socketinfo_t *info = &udp_socket->srv_info;
	int send_len = 0;
    udp_piece_t *udp_piece = udp_piece_init(udp_socket->piece_capacity);
	int	pieces = udp_piece_cut(udp_piece, buf, len);
	LOG_INF("send pieces: pieces=%d, len=%d", pieces, len);

	for(int i = 0; i < pieces; i++)
	{
        int size;
        uint8_t *buf = udp_piece_get(udp_piece, pieces - i - 1, &size);
        LOG_INF("send piece[%d]: buf=%p, len=%d", i + 1, buf, size);
        send_len = ::sendto(info->sockfd, buf, size, 0, (struct sockaddr *)&info->addr, sizeof(info->addr));
        if(send_len != size)
        {
            LOG_ERR("An error occurred in this call of sendto, need send len=%d, but actual len=%d!", size, send_len);
            return -1;
        }
	}

    return 0;
}
