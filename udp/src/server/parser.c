#include "prefixhead.h"
#include "session.h"

/*
 *	解析器: TODO-此处应该使用网络流式处理，并做加密解密
*/

void* jvt_parser_encode(jvt_session_t *session, void *buf, int len, int *real_len)
{
	// pt_packet_head_t *head = (pt_packet_head_t *)buf;
	// switch (head->opcode)
	// {
	// case downloadfile_ack:  break;
	// case transferfile_ack:  break;
	// default: break;
	// }

	*real_len = len;

	return buf;
}

int jvt_parser_decode(jvt_session_t *session, void *buf, int len)
{
	pt_packet_head_t *head = (pt_packet_head_t *)buf;
	
	switch (head->opcode)
	{
	case downloadfile_req:  jvt_session_recv_downloadfile_req(session, (pt_downloadfile_req *)buf); break;
	case transferfile_ack:	jvt_session_recv_transferfile_ack(session, (pt_transferfile_ack *)buf); break;
	default:
		LOG_INF("recv: '%s'", (char *)buf);
		return -1;
	}

	return 0;
}