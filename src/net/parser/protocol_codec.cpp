#include "../prefixhead.h"
#include "protocol_codec.h"
#include "../connection.h"

CProtocolCodec::CProtocolCodec()
{
}

CProtocolCodec::~CProtocolCodec()
{
}


int CProtocolCodec::encode(const IPacket* packet, char* out_data, int& out_size)
{
	return 0;
}

IPacket* CProtocolCodec::decode(unsigned short id, const char* data, int size) 
{
	return NULL; 
}

int CProtocolCodec::encode(const char* in_data, int in_len, char* out_data, int& out_len)
{
	return 0; 
}

int CProtocolCodec::decode(const char* in_data, int in_len, char* out_data, int& out_len) 
{
	return 0; 
}
