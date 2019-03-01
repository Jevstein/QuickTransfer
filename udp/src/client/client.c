#include "prefixhead.h"
#include "client.h"
#include "../udp_piece/udp_piece.h"

int jvt_client_init(jvt_client_t *S, char *ip, int port)
{
	assert(S);

	int ret = jvt_session_init(&S->session_, ip, port);
	if (ret < 0)
	{
		LOG_ERR("failed to init session! ret=%d, port=%d", ret, port);
		return -1;
	}

	// LOG_INF("jvt_client_init success! port=%d", port);
	return 0;
}

void jvt_client_run(jvt_client_t *S)
{
	assert(S);
 
	LOG_INF("client start success..., connect port: %d", S->session_.udp_socket_.port);

	do
	{
		jvt_session_run(&S->session_);
	} while (0);
}

void jvt_client_uninit(jvt_client_t *S)
{
	LOG_ERR("TODO: jvt_client_uninit...");
}