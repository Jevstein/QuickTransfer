/***************************************************************************** 
 *  @COPYRIGHT NOTICE 
 *  @Copyright (c) 2017, Microbeam software technology (Shanghai) co. LTD 
 *  @All rights reserved 
 
 *  @file     : PrefixHead.h 
 *  @version  : ver 1.0 
 
 *  @author   : yiwenqiang 
 *  @date     : 2019/1/7 17:33 
 *  @brief    : brief 
*****************************************************************************/ 

#ifndef _PREFIX_HEAD_H_
#define _PREFIX_HEAD_H_
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <iostream>
#include <time.h>
#include <unistd.h>
#include <functional>
#include <dlfcn.h>
#include <assert.h>

#include <string.h>
#include <string>
#include <sstream>
#include <fstream>  
#include <vector>
#include <list>
#include <map>
#include <set>
#include <algorithm>
using namespace std;

#include <signal.h>
#include <errno.h>
#include <netinet/in.h>
//#include <sys/timeb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

#include "../../inc/protocol/protocol.h"
#include "../../inc/protocol/packet.h"
#include "../transfer/transfer.h"

// singleton
#define SINGLETON(type) \
static inline type* GetInstancePtr() \
{ \
	static type _instance; \
	return &_instance; \
}

// delete
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
#define ARRAY(a)	sizeof(a)/sizeof(a[0])

// network message
#define DECLARE_MESSAGE_MAP()			int OnMessage(const char* pData, int nLen);
#define BEGIN_MESSAGE_MAP(thisClass)	int thisClass::OnMessage(const char* pData, int nLen) {\
											CInputStream is(pData, nLen); \
											short opcode = 0; \
											is >> opcode; \
											is.Reset(); \
											switch (opcode)\
											{
#define HANDLE_MESSAGE(id, packet)			case id: DISPATCH_MESSAGE(packet); break;
#define END_MESSAGE_MAP()					default: return opcode;\
											}\
											return 0; \
										}

// error exit
#define ERR_EXIT(m) 		\
    do { 					\
    	perror(m); 			\
    	exit(EXIT_FAILURE); \
    } while (0)

// log
#define LOG_ERR(fmt,...) { time_t now = time(NULL); printf("[%.24s][error][clt] "fmt" ["__FILE__":%s().%d]\n", ctime(&now), ##__VA_ARGS__, __FUNCTION__, __LINE__); }
#define LOG_EXP(fmt,...) { time_t now = time(NULL); printf("[%.24s][exception][clt] "fmt" ["__FILE__":%s().%d]\n", ctime(&now), ##__VA_ARGS__, __FUNCTION__, __LINE__); }
#define LOG_WAR(fmt,...) { time_t now = time(NULL); printf("[%.24s][warning][clt] "fmt"\n", ctime(&now), ##__VA_ARGS__); }
#define LOG_INF(fmt,...) { time_t now = time(NULL); printf("[%.24s][inf][clt] "fmt"\n", ctime(&now), ##__VA_ARGS__); }
#define LOG_DBG(fmt,...) { time_t now = time(NULL); printf("[%.24s][dbg][clt] "fmt"\n", ctime(&now), ##__VA_ARGS__); }

// ip and port
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6666

#endif //_PREFIX_HEAD_H_
