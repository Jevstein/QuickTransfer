#ifndef _INNER_H_
#define _INNER_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>
#include <time.h>
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>
#include <arpa/inet.h> 

// log
#define LOG_ERR(fmt,...) { time_t now = time(NULL); printf("[%.24s][error][udp] "fmt" ["__FILE__":%s().%d]\n", ctime(&now), ##__VA_ARGS__, __FUNCTION__, __LINE__); }
#define LOG_EXP(fmt,...) { time_t now = time(NULL); printf("[%.24s][exception][udp] "fmt" ["__FILE__":%s().%d]\n", ctime(&now), ##__VA_ARGS__, __FUNCTION__, __LINE__); }
#define LOG_WAR(fmt,...) { time_t now = time(NULL); printf("[%.24s][warning][udp] "fmt"\n", ctime(&now), ##__VA_ARGS__); }
#define LOG_INF(fmt,...) { time_t now = time(NULL); printf("[%.24s][inf][udp] "fmt"\n", ctime(&now), ##__VA_ARGS__); }
#define LOG_DBG(fmt,...) { time_t now = time(NULL); printf("[%.24s][dbg][udp] "fmt"\n", ctime(&now), ##__VA_ARGS__); }


#endif//_INNER_H_
