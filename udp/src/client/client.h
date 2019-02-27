
#ifndef _CLIENT_H_
#define _CLIENT_H_
#include "../udp_piece/udp_socket.h"

typedef struct _jvt_client
{
    udp_socket_t udp_socket_;
} jvt_client_t;


int jvt_client_init(jvt_client_t *S, int port);
void jvt_client_run(jvt_client_t *S);
void jvt_client_uninit(jvt_client_t *S);

#endif //_CLIENT_H_