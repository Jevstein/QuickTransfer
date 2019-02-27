#include "prefixhead.h"
#include "server.h"

void _on_recv_data(void* user_data, udp_socket_result_t* result)
{
	assert(result);
	jvt_server_t* S = (jvt_server_t *)user_data;

	if (result->type == RECV_TYPE_SUCCESS)
	{
		//TODO: parser
		pt_downloadfile_req *req = (pt_downloadfile_req *)result->data;
        LOG_INF("recv[%d]: data=%p, opcode=%d, filename='%s'"
		, result->len, result->data, req->opcode, req->filename);
	}
	else if (result->type == RECV_TYPE_INCOMPLETE)
	{
        // LOG_INF("recv: type=%d", result->type, result->data, result->len);
	}
	else
	{
        LOG_ERR("recv: error=%d, data=%p, len=%d", result->type, result->data, result->len);
	}
}

int jvt_server_init(jvt_server_t *S, int port)
{
	assert(S);

	int ret = 0;

	S->udp_socket_.srv_info.port = port;
	S->udp_socket_.recv_func_callback = _on_recv_data;
	S->udp_socket_.user_data = S;
	ret = udp_socket_init(&S->udp_socket_);
	if (ret < 0)
	{
		LOG_ERR("failed to init socket! ret=%d, port=%d", ret, port);
		return ret;
	}

	ret = udp_socket_bind(&S->udp_socket_);
	if (ret < 0)
	{
		LOG_ERR("failed to bind socket! ret=%d, port=%d", ret, port);
		return ret;
	}

	LOG_INF("jvt_server_init success! port=%d", port);
	return 0;
}

void jvt_server_run(jvt_server_t *S)
{
	assert(S);
	udp_socket_loop(&S->udp_socket_);
}

void jvt_server_uninit(jvt_server_t *S)
{
	LOG_INF("uninit ...");
}