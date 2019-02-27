#include "prefixhead.h"
#include "session.h"

// void _on_recv_data(void* user_data, udp_socket_result_t* result)
// {
// 	assert(result);
// 	jvt_session_t* S = (jvt_session_t *)user_data;

// 	if (result->type == RECV_TYPE_SUCCESS)
// 	{
// 		//TODO: parser
// 		pt_downloadfile_req *req = (pt_downloadfile_req *)result->data;
//         LOG_INF("recv[%d]: data=%p, opcode=%d, filename='%s'"
// 		, result->len, result->data, req->opcode, req->filename);
// 	}
// 	else if (result->type == RECV_TYPE_INCOMPLETE)
// 	{
//         // LOG_INF("recv: type=%d", result->type, result->data, result->len);
// 	}
// 	else
// 	{
//         LOG_ERR("recv: error=%d, data=%p, len=%d", result->type, result->data, result->len);
// 	}
// }

int jvt_session_init(jvt_session_t *session, int port)
{
	assert(session);

	return 0;
}


void jvt_session_uninit(jvt_session_t *S)
{
}