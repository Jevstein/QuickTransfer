
#ifndef _SERVER_H_
#define _SERVER_H_
#include "../udp_piece/udp_socket.h"
#include "session.h"

typedef struct _jvt_server
{
    jvt_session_t *sessions_[JVT_MAX_SESSIONS];
    udp_socket_t udp_socket_;
} jvt_server_t;


int jvt_server_init(jvt_server_t *S, int port);
void jvt_server_run(jvt_server_t *S);
void jvt_server_uninit(jvt_server_t *S);

#endif //_SERVER_H_