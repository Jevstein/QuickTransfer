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

class CProtocolCodec : public IPacketParser
{
public:
	CProtocolCodec();
	virtual ~CProtocolCodec();
    virtual int encode(IPacket* packet, char* out_data, int& out_size);
    virtual IPacket* decode(const char* data, int size, int& out_size);
};

#endif//_PROTOCOL_CODEC_H_
