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

//pt_id_downloadfile_ack
NET_PACKET(pt_id_downloadfile_req)
{
	std::string filename;
};
//pt_id_downloadfile_ack
NET_PACKET(pt_id_downloadfile_ack)
{
	std::string filename;
	int total_size;
};


//START!!! Automatically generated implement follow

//pt_id_downloadfile_req
NET_GET(pt_id_downloadfile_req)
{
	return is >> pt.opcode >> pt.filename;
}
NET_PUT(pt_id_downloadfile_req)
{
	return os << pt.opcode << pt.filename;
}
//pt_id_downloadfile_ack
NET_GET(pt_id_downloadfile_ack)
{
	return is >> pt.opcode >> pt.filename >> pt.total_size;
}																
NET_PUT(pt_id_downloadfile_ack)
{																
	return os << pt.opcode << pt.filename << pt.total_size
}
//END!!! Automatically generated implement follow

#endif//_PACKET_H_