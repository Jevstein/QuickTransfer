#include "prefixhead.h"
#include "client.h"


int jvt_client_init(jvt_client_t *S, int port)
{
	assert(S);

	S->dn_socket_.srv_port = port;
	int ret = dn_socket_init(&S->dn_socket_);
	if (ret < 0)
	{
		LOG_ERR("failed to init socket! port=%d", port);
		return 0;
	}

	LOG_INF("jvt_client_init success! port=%d", port);
	return 0;
}

void jvt_client_run(jvt_client_t *S)
{
	assert(S);
	dn_socket_loop(&S->dn_socket_);
}

void jvt_client_uninit(jvt_client_t *S)
{
	LOG_INF("uninit ...");
}