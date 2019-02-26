/*
 *  log.h 
 *  log
 *
 *  Created by Jevstein on 2019/01/11 10:20.
 *  Copyright @ 2018year Jevstein. All rights reserved.
 *
 */

#ifndef __LOG_H__
#define __LOG_H__

#include <time.h>

// log - cpp4log
#define LOG_ERR(fmt,...) { time_t now = time(NULL); printf("[%.24s][error] "fmt" ["__FILE__":%s().%d]\n", ctime(&now), ##__VA_ARGS__, __FUNCTION__, __LINE__); }
#define LOG_EXP(fmt,...) { time_t now = time(NULL); printf("[%.24s][exception] "fmt" ["__FILE__":%s().%d]\n", ctime(&now), ##__VA_ARGS__, __FUNCTION__, __LINE__); }
#define LOG_WAR(fmt,...) { time_t now = time(NULL); printf("[%.24s][warning] "fmt"\n", ctime(&now), ##__VA_ARGS__); }
#define LOG_INF(fmt,...) { time_t now = time(NULL); printf("[%.24s][inf] "fmt"\n", ctime(&now), ##__VA_ARGS__); }
#define LOG_DBG(fmt,...) { time_t now = time(NULL); printf("[%.24s][dbg] "fmt"\n", ctime(&now), ##__VA_ARGS__); }

#endif//__LOG_H__