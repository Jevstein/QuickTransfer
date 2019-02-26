/*
 *  PacketParser.h 
 *  PacketParser
 *
 *  Created by Jevstein on 2018/4/17 17:52.
 *  Copyright @ 2018year Jevstein. All rights reserved.
 *	default packet parser
 */
#ifndef _PACKET_PARSER_H_
#define _PACKET_PARSER_H_

class CPacketParser : public IPacketParser
{
	enum
	{
		KeySize = 16,
	};
    
public:
	CPacketParser();
	virtual ~CPacketParser();
    virtual int encode(const IPacket* packet, char* out_data, int& out_size);
    virtual int decode(IPacket *out_packet, int& max_len, const char* data, int size);
	virtual int encode(const char* in_data, int in_len, char* out_data, int& out_len);
	virtual int decode(const char* in_data, int in_len, char* out_data, int& out_len);
    
private:
	unsigned char symmetric_key_[16];
	ushort  sh_seed_;
};

#endif//_PACKET_PARSER_H_
