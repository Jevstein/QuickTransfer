//
//  PrefixHead.h
//  YiTechStudio001
//
//  Created by Jevstein on 2018/3/27.
//  Copyright © 2018年 Jevstein. All rights reserved.
//

#ifndef _PREFIX_HEAD_H_
#define _PREFIX_HEAD_H_

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <list>
#include <map>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>

#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "../common/osversion.h"


#define LOG_ERR(fmt,...) log__.error(fmt" ["__FILE__":%s().%d]", ##__VA_ARGS__, __YI_FUNC__, __LINE__)
#define LOG_EXP(fmt,...) log__.excpt(fmt" ["__FILE__":%s().%d]", ##__VA_ARGS__, __YI_FUNC__, __LINE__)
#define LOG_WAR(fmt,...) log__.warn(fmt, ##__VA_ARGS__)
#define LOG_DBG(fmt,...) //log__.debug(fmt, ##__VA_ARGS__)
//#define LOG_DBG(fmt,...) log__.debug(__FILE__"[%p]: "fmt, this, ##__VA_ARGS__)

#endif //_PREFIX_HEAD_H_
