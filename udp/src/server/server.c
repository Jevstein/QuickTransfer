#include "prefixhead.h"
#include "server.h"

// void _on_recv_data(void* user_data, udp_socket_result_t* result)
// {
// 	assert(result);
// 	jvt_server_t* S = (jvt_server_t *)user_data;

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

udp_socket_t* on_find_udp_socket(udp_socket_t *udp_socket, char* ip, int port)
{
	jvt_server_t *S = (jvt_server_t *)udp_socket->callback.session;

	udp_socket_t* udp_socket_new = NULL;

	// 1.查找已存在的客户端连接
	int i, j = -1;
	int count = sizeof(S->sessions_) / sizeof(jvt_session_t*);
	for (i = 0; i < count; i++)
	{
		if (!S->sessions_[i])
		{
			if (j < 0)
				j = i;
			continue;
		}
		
		udp_socket_new = &S->sessions_[i]->udp_socket_;
		if ((strcmp(udp_socket_new->ip, ip) == 0) && (udp_socket_new->port == port))
			return udp_socket_new;
	}

	// 2.未找到
	// 2.1.没有足够的会话空间
	if (j == -1) 
	{
		LOG_INF("no enough space for session!");
		return NULL;
	}

	// 2.2.新建新的会话
	S->sessions_[i] = (jvt_session_t *)calloc(1, sizeof(jvt_session_t));
	assert(S->sessions_[i]);

	//TODO: 创建udp_socket_t*

	return &S->sessions_[i]->udp_socket_;
}


int jvt_server_init(jvt_server_t *S, int port)
{
	assert(S);

	// 1.初始化S->sessions_
	int i;
	for (i = 0; i < (sizeof(S->sessions_) / sizeof(jvt_session_t*)); i++)
	{
		S->sessions_[i] = NULL;
	}

	// 2.初始化S->udp_socket_
	S->udp_socket_.callback.session = S;
    S->udp_socket_.callback.find_udp_socket_func = on_find_udp_socket;
    S->udp_socket_.callback.create_session_func = NULL;
    S->udp_socket_.callback.destroy_session_func = NULL;
    S->udp_socket_.callback.recv_data_func = NULL;
	int ret = udp_socket_init(&S->udp_socket_, NULL, port, JVT_PIECE_CAPACITY);
	if (ret < 0)
	{
		LOG_ERR("failed to init socket! ret=%d, port=%d", ret, port);
		return ret;
	}

	// 3.绑定
	ret = udp_socket_bind(&S->udp_socket_);
	if (ret < 0)
	{
		LOG_ERR("failed to bind socket! ret=%d, port=%d", ret, port);
		return ret;
	}

	LOG_INF("bind: port=%d", port);
	return 0;
}

void jvt_server_run(jvt_server_t *S)
{
	assert(S);

	LOG_INF("run ...");
	while (1) 
	{
		udp_socket_recv(&S->udp_socket_);
	}
}

void jvt_server_uninit(jvt_server_t *S)
{
	udp_socket_uninit(&S->udp_socket_);

	int i;
	for (i = 0; i < (sizeof(S->sessions_) / sizeof(jvt_session_t*)); i++)
	{
		if (S->sessions_[i])
			free(S->sessions_[i]);
	}

	LOG_INF("end ...");
}