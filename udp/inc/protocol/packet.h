/*
 *  packet.h 
 *  packet
 *
 *  Created by Jevstein on 2018/6/29 12:01.
 *  Copyright @ 2018year Jevstein. All rights reserved.
 *
 */
#ifndef _PACKET_H_
#define _PACKET_H_

#define FILE_PACKET (1024 * 5)
#define NET_PACKET(pt) 					\
	struct pt : public pt_packet_head_t

struct pt_packet_head_t
{
	short opcode;
};

NET_PACKET(pt_downloadfile_req)
{
	char filename[256];
};

NET_PACKET(pt_downloadfile_ack)
{
	char ret;	//返回值: 0=成功，-1=不存在
	int fileid;
	int filesize;
	char filename[256];
};

NET_PACKET(pt_transferfile_noti)
{
	int fileid;
	int index;
	int size;
	char data[FILE_PACKET];
};

NET_PACKET(pt_transferfile_ack)
{
	char ret;	//返回值: 0=成功，-1=失败
	int fileid;
	int index;
};

#endif//_PACKET_H_