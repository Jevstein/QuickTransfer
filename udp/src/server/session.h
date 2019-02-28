
#ifndef _SESSION_H_
#define _SESSION_H_
#include "../udp_piece/udp_socket.h"
#include "file.h"

typedef struct _jvt_session
{
    jvt_file_t files_[JVT_MAX_FILES];
    udp_socket_t udp_socket_;
} jvt_session_t;


int jvt_session_init(jvt_session_t *S);
void jvt_session_uninit(jvt_session_t *S);

#endif //_SERVER_H_