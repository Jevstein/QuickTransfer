#include "inner.h"
#include "udp_socket.h"
#include "udp_piece.h"
#include <sys/types.h>
#include <sys/socket.h>

#define MAX_MSG_SIZE (256 * 1024) /*recv缓冲器大小*/

#define RECV_CALLBACK(s, t, d, l) {         \
    udp_socket_result_t r;                  \
    r.type = t;                             \
    r.data = d;                             \
    r.len = l;                              \
    s->recv_data_func(s->user_data, &r);    \
}

typedef struct _udp_socketinfo
{
    int sockid;             //标识: ip与port映射成的唯一标识，用于区分不同的客户连接
    int sockfd;             //socket描述符
    struct sockaddr_in addr;//地址
    udp_piece_t *udp_piece; //UDP分片
} udp_socketinfo_t;

int udp_socket_init(udp_socket_t *udp_socket, const char *ip, int port, int piece_size)
{
    assert(udp_socket);

    if (ip) 
        strcpy(udp_socket->ip, ip);
    else 
        memset(udp_socket->ip, 0, sizeof(udp_socket->ip));
    udp_socket->port = port;
    udp_socket->max_piece_size = piece_size;

    udp_socket->info = (udp_socketinfo_t *)calloc(1, sizeof(udp_socketinfo_t));
    assert(udp_socket->info);

    if (udp_socket->info)
    {
        udp_socket->info->sockid = 0;
        
        udp_socket->info->sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (udp_socket->info->sockfd < 0)
            return -1;

        bzero(&udp_socket->info->addr, sizeof(udp_socket->info->addr));
        udp_socket->info->addr.sin_family = AF_INET;
        udp_socket->info->addr.sin_addr.s_addr = (ip==NULL) ? htonl(INADDR_ANY) : inet_addr(ip);
        udp_socket->info->addr.sin_port = htons(port);

        //setsockopt(udp_socket->info->sockfd, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(timeval))；

        udp_socket->info->udp_piece = udp_piece_init(udp_socket->max_piece_size);
        assert(udp_socket->info->udp_piece);
    }

    return 0;
}

void udp_socket_uninit(udp_socket_t *udp_socket)
{
    if (!udp_socket)
        return;

    memset(udp_socket->ip, 0, sizeof(udp_socket->ip));
    udp_socket->port = 0;
    udp_socket->max_piece_size = 0;

    if (udp_socket->info)
    {
        close(udp_socket->info->sockfd);
        udp_piece_deinit(udp_socket->info->udp_piece);
        free(udp_socket->info);
    }
}

// void udp_socket_destroy(udp_socket_t *udp_socket)
// {
//     if (!udp_socket)
//         return;

//     udp_socket_uninit(udp_socket);
//     free(udp_socket);
// }

int udp_socket_bind(udp_socket_t *udp_socket)
{
    assert(udp_socket || udp_socket->info);

    if (bind(udp_socket->info->sockfd, (struct sockaddr *)&udp_socket->info->addr, sizeof(udp_socket->info->addr)) < 0)
        return -1;

    return 0;
}

int udp_socket_send(udp_socket_t *udp_socket, const void *buf, int len)
{
    assert(udp_socket || udp_socket->info || udp_socket->info->udp_piece);

    udp_socketinfo_t *info = udp_socket->info;
    udp_piece_t *udp_piece = udp_socket->info->udp_piece;

	int send_len = 0;
	int	pieces = udp_piece_cut(udp_piece, buf, len);
	LOG_INF("send pieces: pieces=%d, len=%d", pieces, len);

	for(int i = 0; i < pieces; i++)
	{
        int size;
        uint8_t *buf = udp_piece_get(udp_piece, pieces - i - 1, &size);
        LOG_INF("send piece[%d]: buf=%p, len=%d", i + 1, buf, size);
        send_len = sendto(info->sockfd, buf, size, 0, (struct sockaddr *)&info->addr, sizeof(info->addr));
        if(send_len != size)
        {
            LOG_ERR("An error occurred in this call of sendto, need send len=%d, but actual len=%d!", size, send_len);
            return -1;
        }
	}

    return 0;
}

int udp_socket_recv(udp_socket_t *udp_socket)
{
    assert(udp_socket || udp_socket->info || udp_socket->info->udp_piece);
    
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(struct sockaddr_in);
    char ip[64] = {0};

    unsigned char buf[MAX_MSG_SIZE];
    int len = 0;

    while(1)
    {
        // 1.接收数据
        len = recvfrom(udp_socket->info->sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&addr, &addr_len);
        if(len < 0)
        {
			LOG_ERR("failed to recvfrom: len=%d", len);
            return -1;
        }

        if (len == 0)
        {
			LOG_ERR("failed to recvfrom: len=%d", len);
            continue;
        }

        inet_ntop(AF_INET, &addr.sin_addr, ip, sizeof(ip));
        LOG_INF("sockfd=%d, client [%s:%d]", udp_socket->info->sockfd, ip, ntohs(addr.sin_port));

        // 2.找到客户连接：未找到则新建
        udp_socket_t* udp_socket_client = udp_socket->callback.find_udp_socket_func(udp_socket, ip, ntohs(addr.sin_port));
        assert(udp_socket_client);

        udp_piece_t *udp_piece = udp_socket_client->info->udp_piece;
        assert(udp_piece);

        // 3.重组分片
        int ret = udp_piece_merge_ex(udp_piece, buf, len);
        if (ret == 0) {//continue
            LOG_INF("recv: ret=%d, len=%d", ret, len);
            // RECV_CALLBACK(udp_socket, RECV_TYPE_INCOMPLETE, recvbuf, len);
            continue;
        } else if (ret == 1) {//finish
            LOG_INF("received：msg='%s', len=%d", udp_piece->recv_buf, udp_piece->recv_len);
            // RECV_CALLBACK(udp_socket, RECV_TYPE_SUCCESS, udp_piece->recv_buf, udp_piece->recv_len);
            udp_piece_reset(udp_piece);
            break;
        } else if(ret == -1) {//error
            LOG_ERR("exception of udp piece!");
            // RECV_CALLBACK(udp_socket, RECV_TYPE_EXCEPTION, recvbuf, len);
            udp_piece_reset(udp_piece);
            return 1;
        } else {//unknown
            LOG_ERR("unknown exception of udp piece!");
            // RECV_CALLBACK(udp_socket, RECV_TYPE_UNKNOWN, recvbuf, len);
            udp_piece_reset(udp_piece);
            return 2;
        }
    }

    return 0;
}