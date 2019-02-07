/*
 *  protocol_codec.h 
 *  protocol_codec
 *
 *  Created by Jevstein on 2019/1/19 17:52.
 *  Copyright @ 2018year Jevstein. All rights reserved.
 *	
 */
#ifndef _PROTOCOL_CODEC_H_
#define _PROTOCOL_CODEC_H_
#include "../net_intf.h"

class CProtocolCodec : public IPacketParser
{
public:
	CProtocolCodec();
	virtual ~CProtocolCodec();
    virtual int encode(const IPacket* packet, char* out_data, int& out_size);
    virtual IPacket* decode(unsigned short id, const char* data, int size);
	virtual int encode(const char* in_data, int in_len, char* out_data, int& out_len);
	virtual int decode(const char* in_data, int in_len, char* out_data, int& out_len);
    // virtual int encode(const IPacket* packet, char* out_data, int& out_size) { return 0; }
    // virtual IPacket* decode(unsigned short id, const char* data, int size) { return NULL; }
	// virtual int encode(const char* in_data, int in_len, char* out_data, int& out_len) { return 0; }
	// virtual int decode(const char* in_data, int in_len, char* out_data, int& out_len) { return 0; }
};

#endif//_PROTOCOL_CODEC_H_
