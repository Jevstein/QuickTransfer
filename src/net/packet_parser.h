/*
 *  NetPacketParser.h 
 *  NetPacketParser
 *
 *  Created by Jevstein on 2018/4/17 17:52.
 *  Copyright @ 2018year Jevstein. All rights reserved.
 *
 */
#ifndef _NET_PACKET_PARSER_H_
#define _NET_PACKET_PARSER_H_

class CNetPacketParser : public IPacketParser
{
	enum
	{
		KeySize = 16,
	};
    
public:
	CNetPacketParser(void);
	virtual ~CNetPacketParser(void);
	virtual int encode(const char* in_data, int in_len, char* out_data, int& out_len);
	virtual int decode(const char* in_data, int in_len, char* out_data, int& out_len);
    
private:
	unsigned char symmetric_key_[16];
	ushort  sh_seed_;
};

#endif//_NET_PACKET_PARSER_H_
