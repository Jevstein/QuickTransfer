
#ifndef _SESSION_H_
#define _SESSION_H_
#include "file.h"

typedef struct _jvt_session
{
    jvt_file_t files_[JVT_MAX_FILES];//应该考虑红黑树
    udp_socket_t udp_socket_;
} jvt_session_t;


int jvt_session_init(jvt_session_t *S, char *ip, int port);
void jvt_session_uninit(jvt_session_t *S);
void jvt_session_run(jvt_session_t *S);

void jvt_session_recv_downloadfile_ack(jvt_session_t *S, pt_downloadfile_ack *ack);
void jvt_session_recv_transferfile_noti(jvt_session_t *S, pt_transferfile_noti *noti);

#endif //_SERVER_H_