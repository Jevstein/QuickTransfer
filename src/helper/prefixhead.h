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
#include <fcntl.h>

#include "globaldefine.h"
// #include "log.h"

// log 
#define LOG_ERR(fmt,...) { time_t now = time(NULL); printf("[%.24s][error][helper] "fmt" ["__FILE__":%s().%d]\n", ctime(&now), ##__VA_ARGS__, __FUNCTION__, __LINE__); }
#define LOG_EXP(fmt,...) { time_t now = time(NULL); printf("[%.24s][exception][helper] "fmt" ["__FILE__":%s().%d]\n", ctime(&now), ##__VA_ARGS__, __FUNCTION__, __LINE__); }
#define LOG_WAR(fmt,...) { time_t now = time(NULL); printf("[%.24s][warning][helper] "fmt"\n", ctime(&now), ##__VA_ARGS__); }
#define LOG_INF(fmt,...) { time_t now = time(NULL); printf("[%.24s][inf][helper] "fmt"\n", ctime(&now), ##__VA_ARGS__); }
#define LOG_DBG(fmt,...) { time_t now = time(NULL); printf("[%.24s][dbg][helper] "fmt"\n", ctime(&now), ##__VA_ARGS__); }

#endif //_PREFIX_HEAD_H_
