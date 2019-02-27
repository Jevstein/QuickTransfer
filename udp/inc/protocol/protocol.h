#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

#define JVT_PIECE_CAPACITY (64 * 1024)	//udp分片容量
#define JVT_MAX_PATH 256				//文件名大小
#define JVT_MAX_FILES 5					//每个会话最大的文件数量(暂时)
#define JVT_MAX_SESSIONS 5				//最大的UDP连接(暂时)

enum OPCODE 
{
	downloadfile_req 	= 1001, 
	downloadfile_ack 	= 1002, 
	transferfile_noti 	= 1003, 
	transferfile_ack 	= 1004, 
};


#endif//_PROTOCOL_H_