<<<<<<< HEAD
﻿/***************************************************************************** 
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
#include <netinet/in.h>
//#include <sys/timeb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
//#include <mysql.h>
#include <gd.h>
#include <gdfontg.h>
#include <gdfonts.h>
#include <cassert>
#include <boost/assert.hpp>

#include "Util.h"

#include <BaseModule/BD_BaseHelper/modulefactory.h>
#include <BaseModule/BD_BaseHelper/globaldefine.h>
#include <BaseModule/BD_BaseHelper/netstream.h>
#include <BaseModule/BD_BaseHelper/defpacketdefine.h>
#include <BaseModule/BD_BaseHelper/log.h>
#include <BaseModule/BD_BaseHelper/tinystr.h>
#include <BaseModule/BD_BaseHelper/tinyxml.h>
#include <BaseModule/BD_BaseHelper/objectpool.h>
#include <BaseModule/BD_BaseHelper/utilty.h>
#include <BaseModule/BD_BaseHelper/mymd5.h>
#include <BaseModule/BD_BaseHelper/Thread.h>
#include <BaseModule/BD_BaseHelper/gmock/gmock.h>
#include <BaseModule/BD_BaseHelper/gtest/gtest.h>
#include <BaseModule/BD_BaseHelper/Base64.h>
#include <BaseModule/BD_Net.h>
//#include <BaseModule/BD_DB.h>
//#include <BaseModule/BD_AuthenComp.h>
#include <BaseModule/BD_Logger.h>

#include "Protocal/AuthenServer/opcode.h"
#include "Protocal/AuthenServer/packetdefine.h"
#include "Protocal/ShopmallServer/opcode.h"
#include "Protocal/ShopmallServer/packetdefine.h"
#include "Protocal/DirServer/opcode.h"
#include "Protocal/DirServer/packetdefine.h"
#include "Protocal/CenterServer/opcode.h"
#include "Protocal/CenterServer/packetdefine.h"
//#include "Protocal/MatchServerWJ/opcode.h"
//#include "Protocal/MatchServerWJ/packetdefine.h"
#include "Protocal/ServerModule/opcode.h"
#include "Protocal/ServerModule/packetdefine.h"
//#include "Protocal/MatchModuleWJ/opcode.h"
//#include "Protocal/MatchModuleWJ/packetdefine.h"
//#include "Protocal/ClubServer/opcode.h"
//#include "Protocal/ClubServer/packetdefine.h"

//// delete
//#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
//#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
//#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
//#define ARRAY(a)	sizeof(a)/sizeof(a[0])

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

// log
#define LOG_ERR(fmt,...) printf("[error] "fmt" ["__FILE__":%s().%d]\n", ##__VA_ARGS__, __FUNCTION__, __LINE__)
#define LOG_EXP(fmt,...) printf("[exception] "fmt" ["__FILE__":%s().%d]\n", ##__VA_ARGS__, __FUNCTION__, __LINE__)
#define LOG_WAR(fmt,...) printf("[warning] "fmt"\n", ##__VA_ARGS__)
#define LOG_INF(fmt,...) printf("[inf] "fmt"\n", ##__VA_ARGS__)
#define LOG_DBG(fmt,...) printf("[dbg] "fmt"\n", ##__VA_ARGS__)

// AuthenServer ip and port
#define SRV_AUTHENSERVER_IP "127.0.0.1"//"61.172.241.239"
#define SRV_AUTHENSERVER_PORT 8000

// DirServer ip and port
#define SRV_DIRSERVER_IP "127.0.0.1"//"61.172.241.239"
#define SRV_DIRSERVER_PORT 8101

#endif //_PREFIX_HEAD_H_
=======
﻿//
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

#include "../../common/osversion.h"


#define LOG_ERR(fmt,...) log__.error(fmt" ["__FILE__":%s().%d]", ##__VA_ARGS__, __YI_FUNC__, __LINE__)
#define LOG_EXP(fmt,...) log__.excpt(fmt" ["__FILE__":%s().%d]", ##__VA_ARGS__, __YI_FUNC__, __LINE__)
#define LOG_WAR(fmt,...) log__.warn(fmt, ##__VA_ARGS__)
#define LOG_DBG(fmt,...) //log__.debug(fmt, ##__VA_ARGS__)
//#define LOG_DBG(fmt,...) log__.debug(__FILE__"[%p]: "fmt, this, ##__VA_ARGS__)

#endif //_PREFIX_HEAD_H_
>>>>>>> 463f6762fd08749f0c5d89991aee81bc4bbc1cdc
