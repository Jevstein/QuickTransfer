
#ifndef _PARSER_H_
#define _PARSER_H_

typedef struct _jvt_session jvt_session_t;

void* jvt_parser_encode(jvt_session_t *session, void *buf, int len, int *real_len);
int jvt_parser_decode(jvt_session_t *session, void *buf, int len);

#endif //_PARSER_H_