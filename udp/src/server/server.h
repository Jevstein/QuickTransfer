
#ifndef _SERVER_H_
#define _SERVER_H_
#include "session.h"

typedef struct _jvt_server
{
    jvt_session_t *sessions_[JVT_MAX_SESSIONS];//应该考虑红黑树
    udp_socket_t udp_socket_;
    // jvt_net_io_t net_io_;
    jvt_net_reactor_t reactor;
} jvt_server_t;


int jvt_server_init(jvt_server_t *S, int port);
void jvt_server_run(jvt_server_t *S);
void jvt_server_uninit(jvt_server_t *S);

#endif //_SERVER_H_