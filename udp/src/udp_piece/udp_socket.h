#ifndef DN_SOCKET_H
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

typedef struct _udp_socket_result
{
    int type;   //类型: 见RECV_TYPE
    void *data; //数据
    int len;    //长度
} udp_socket_result_t;


typedef void (* recv_data_func_t)(void* user_data, udp_socket_result_t* result);

typedef struct _udp_socketinfo
{
    // int socket;                             // 定义socket句柄
    int sockfd;
    uint32_t ip;
    uint16_t port;
    struct sockaddr_in addr;
} udp_socketinfo_t;

/**
 * 封装socket
 */
typedef struct _udp_socket
{
    recv_data_func_t recv_func_callback;    // 接收回调
    void* user_data;                        // 接收回调上下文

    udp_socketinfo_t srv_info;              //服务端socket信息
    udp_socketinfo_t clt_infos[5];          //客户端socket信息

    int exit_thread;                        // 初始化为0, 等于1时则退出线程

    int piece_capacity;                     //分配容量
} udp_socket_t;

/**
 * @brief 初始化socket
 * @param udp_socket
 * @return
 */
int udp_socket_init(udp_socket_t *udp_socket);

/**
 * @brief 退出socket
 * @param udp_socket
 * @return
 */
void udp_socket_uninit(udp_socket_t *udp_socket);

/**
 * @brief 
 * @param udp_socket
 * @return
 */
int udp_socket_loop(udp_socket_t *udp_socket);

/** 绑定
 * @brief 
 * @param udp_socket
 * @return
 */
int udp_socket_bind(udp_socket_t *udp_socket);

/**
 * @brief 发送数据
 * @param udp_socket
 * @return
 */
int udp_socket_send(udp_socket_t *udp_socket, const void* buf, int size);

#endif // DN_SOCKET_H
