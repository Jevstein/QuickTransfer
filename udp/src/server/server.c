#include "prefixhead.h"
#include "server.h"


int jvt_server_init(jvt_server_t *S, int port)
{
	assert(S);

	S->dn_socket_.srv_port = port;
	int ret = dn_socket_init(&S->dn_socket_);
	if (ret < 0)
	{
		LOG_ERR("failed to init socket! port=%d", port);
		return 0;
	}

	LOG_INF("jvt_server_init success! port=%d", port);
	return 0;
}

void jvt_server_run(jvt_server_t *S)
{
	assert(S);
	dn_socket_loop(&S->dn_socket_);
}

void jvt_server_uninit(jvt_server_t *S)
{
	LOG_INF("uninit ...");
}