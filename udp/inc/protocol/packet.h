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

#define FILE_PACKET (1024 * 4)
#define NET_PACKET(p) 					\
	struct p : public pt_packet_head

struct pt_packet_head
{
	short opcode;
};

NET_PACKET(pt_downloadfile_req)
{
	char filename[256];
};

NET_PACKET(pt_downloadfile_ack)
{
	int fileid;
	int total_size;
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
	int fileid;
	char ret;
	int index;
};

#endif//_PACKET_H_