
#ifndef _BASE64_H_
#define _BASE64_H_

int base64_encode(char* buf, const char* src, int src_len /*= 0*/);
int base64_decode(char* buf, const char* src, int src_len /*= 0*/);

#endif //_BASE64_H_