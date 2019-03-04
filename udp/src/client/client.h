
#ifndef _CLIENT_H_
#define _CLIENT_H_
#include "session.h"

typedef struct _jvt_client
{
    jvt_session_t session_;
} jvt_client_t;


int jvt_client_init(jvt_client_t *S, char *ip, int port);
void jvt_client_run(jvt_client_t *S);
void jvt_client_uninit(jvt_client_t *S);

#endif //_CLIENT_H_