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

#define 548
#define NET_PACKET(p) 					\
	struct p : public pt_packet_head

struct pt_packet_head
{
	int id;
};


NET_PACKET(pt_downloadfile_req)
{
	char filename[256];
};

NET_PACKET(pt_downloadfile_ack)
{
	int total_size;
	char filename[256];
};

NET_PACKET(pt_downloadfile_noti)
{
	char data[];
	int size;
};

#endif//_PACKET_H_