﻿#ifndef DN_SOCKET_H
#define DN_SOCKET_H
#include <stdint.h>
#include <string.h>
#include <netinet/in.h>

enum RECV_TYPE
{
    RECV_TYPE_ERROR     = -1,
    RECV_TYPE_SUCCESS   = 0,
    RECV_TYPE_INCOMPLETE,
    RECV_TYPE_EXCEPTION,
    RECV_TYPE_UNKNOWN,
};

typedef struct _udp_socket_result udp_socket_result_t;
typedef struct _udp_socket_callback udp_socket_callback_t;
typedef struct _udp_socketinfo udp_socketinfo_t;
typedef struct _udp_socket udp_socket_t;

typedef void (* recv_data_func_t)(void* session, udp_socket_result_t* result);
typedef void (* create_session_func_t)(udp_socket_t* udp_socket);
typedef void (* destroy_session_func_t)();
typedef udp_socket_t* (* find_udp_socket_func_t)(udp_socket_t *udp_socket, char* ip, int port);// typedef udp_socket_t* (* find_socket_func_t)(int sockid);

typedef struct _udp_socket_result
{
    int type;   //类型: 见RECV_TYPE
    void *data; //数据
    int len;    //长度
} udp_socket_result_t;

typedef struct _udp_socket_callback
{
    void *session;

    find_udp_socket_func_t find_udp_socket_func;
    create_session_func_t create_session_func;
    destroy_session_func_t destroy_session_func;
    recv_data_func_t recv_data_func;
} udp_socket_callback_t;

typedef struct _udp_socket
{
    char ip[64];                    //IP地址
    uint16_t port;                  //端口
    int max_piece_size;             //分片大小
    udp_socket_callback_t callback; //外部回调
    udp_socketinfo_t *info;         //socket信息[私有成员]
} udp_socket_t;

// udp socket
int udp_socket_init(udp_socket_t *udp_socket, const char *ip, int port, int piece_size);
void udp_socket_uninit(udp_socket_t *udp_socket);
// void udp_socket_destroy(udp_socket_t *udp_socket);
int udp_socket_bind(udp_socket_t *udp_socket);
int udp_socket_send(udp_socket_t *udp_socket, const void* buf, int size);
int udp_socket_recv(udp_socket_t *udp_socket);

#endif // DN_SOCKET_H
