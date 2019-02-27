
#ifndef _FILE_H_
#define _FILE_H_
#include "../udp_piece/udp_socket.h"

typedef struct _jvt_session jvt_session_t;

typedef struct _jvt_fileinfo
{
    char filename[JVT_MAX_PATH];
    int total_size;
} jvt_fileinfo_t;

typedef struct _jvt_file
{
    int fileid_;
    jvt_fileinfo_t fileinfo_;
    jvt_session_t *session_;
} jvt_file_t;


int jvt_file_init(jvt_file_t *F, jvt_session_t *session);
void jvt_file_uninit(jvt_file_t *F);

#endif //_FILE_H_