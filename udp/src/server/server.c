#include "prefixhead.h"
#include "server.h"

udp_socket_t* _on_find_udp_socket(udp_socket_t *udp_socket, char* ip, int port
								, reset_udp_socket_func_t func, void *user_data)
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
		// LOG_DBG("find socket[%p][%d]...: [%s:%d] [%s:%d]", S->sessions_[i], i
		// 	, udp_socket_new->ip, udp_socket_new->port, ip, port);
		if ((strcmp(udp_socket_new->ip, ip) == 0) && (udp_socket_new->port == port))
		{
			// LOG_DBG("find socket success: [%s:%d]", ip, port);
			return udp_socket_new;
		}
	}

	// 2.未找到
	// 2.1.没有足够的会话空间
	if (j == -1) 
	{
		LOG_INF("no enough space for session!");
		return NULL;
	}

	// 2.2.新建新的会话
	S->sessions_[j] = (jvt_session_t *)calloc(1, sizeof(jvt_session_t));
	assert(S->sessions_[j]);
	jvt_session_init(S->sessions_[j]);

	udp_socket_callback_t* cbk = &S->sessions_[j]->udp_socket_.callback;
	if (cbk)
	{
		cbk->session = S->sessions_[j];
		cbk->find_udp_socket_func = NULL;
		cbk->create_session_func = NULL;
		cbk->destroy_session_func = NULL;
		cbk->recv_data_func = jvt_session_on_recv_data;
	}
	func(&S->sessions_[j]->udp_socket_, user_data);

	// LOG_DBG("not find socket: [%p][%d] [%s:%d]", S->sessions_[j], j, S->sessions_[j]->udp_socket_.ip, S->sessions_[j]->udp_socket_.port);

	return &S->sessions_[j]->udp_socket_;
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
    S->udp_socket_.callback.find_udp_socket_func = _on_find_udp_socket;
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

	// 4.反应堆
	ret = jvt_net_reactor_init(&S->reactor);
	if (ret < 0)
	{
		LOG_ERR("failed to init reactor! ret=%d", ret);
		return ret;
	}

	// 5.添加事件
	ret = jvt_net_reactor_add_event(&S->reactor, &S->udp_socket_, EPOLLIN);
	if (ret < 0)
	{
		LOG_ERR("failed to add reactor event[%d]! ret=%d", EPOLLIN, ret);
		return ret;
	}
    
	// LOG_INF("bind: port=%d", port);
	return 0;
}

void jvt_server_run(jvt_server_t *S)
{
	assert(S);

	LOG_INF("server start success..., listen port: %d", S->udp_socket_.port);
	
	//TODO: 正确做法
	// 0.启动反应堆
	// 1.反应堆线程：循环epoll_wait，并将结果以事件的方式保存在队列Q中
	// 2.主线程：循环取出队列Q的事件, 分发处理

	// 临时做法
	jvt_net_reactor_run(&S->reactor);
}

void jvt_server_uninit(jvt_server_t *S)
{
	udp_socket_uninit(&S->udp_socket_);
	jvt_net_reactor_uninit(&S->reactor);

	int i;
	for (i = 0; i < (sizeof(S->sessions_) / sizeof(jvt_session_t*)); i++)
	{
		if (S->sessions_[i])
			free(S->sessions_[i]);
	}

	LOG_ERR("TODO: jvt_server_uninit...");
}