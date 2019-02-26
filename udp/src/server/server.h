
#ifndef _SERVER_H_
#define _SERVER_H_
#include "../udp_piece/dn_socket.h"

typedef struct _jvt_server
{
    dn_socket_t dn_socket_;
} jvt_server_t;


int jvt_server_init(jvt_server_t *S, int port);
void jvt_server_run(jvt_server_t *S);
void jvt_server_uninit(jvt_server_t *S);

#endif //_SERVER_H_