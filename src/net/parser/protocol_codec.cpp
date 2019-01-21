#include "prefixhead.h"
#include "protocol_codec.h"
#include "connection.h"

CProtocolCodec::CProtocolCodec()
{
}

CProtocolCodec::~CProtocolCodec()
{
}

int CProtocolCodec::encode(IPacket* packet, char* out_data, int& out_len)
{
	return out_len;
}

IPacket* CProtocolCodec::decode(const char* data, int size, int& out_size)
{

	return 0;
}

