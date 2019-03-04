
#ifndef _FILE_H_
#define _FILE_H_
#include <sys/mman.h>

#define FILE_BLOCK (1024 * 4)

typedef struct _jvt_session jvt_session_t;

typedef struct _jvt_fileinfo
{
    char filename[JVT_MAX_PATH];
    int filesize;
} jvt_fileinfo_t;

typedef struct _jvt_file
{
    int fileid_;
    jvt_fileinfo_t fileinfo_;
    jvt_session_t *session_;

    void *data_;
    int block_;
} jvt_file_t;


int jvt_file_new_file_id();

int jvt_file_init(jvt_file_t *F, char* name, jvt_session_t *session);
void jvt_file_uninit(jvt_file_t *F);

char* jvt_file_read(jvt_file_t *F, int block, int *len);

#endif //_FILE_H_