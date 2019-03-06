#include "inner.h"
#include "net_reactor.h"

int jvt_net_reactor_watch(jvt_net_reactor_t *reactor);

void* thread_proc(void* argv)
{
	jvt_net_reactor_t *reactor = (jvt_net_reactor_t *)argv;
	assert(reactor);

	jvt_net_reactor_watch(reactor);

    return NULL;  
}

int _on_recv(jvt_net_reactor_t *reactor, udp_socket_t* udp_socket)
{
	//TODO:
	// 接收数据，并加入recv_events队列

	//临时处理
	udp_socket_recv(udp_socket);

	return 0;
}

int _on_send(jvt_net_reactor_t *reactor, udp_socket_t* udp_socket)
{
	//TODO:
	// 取出send_events事件，并发�?

	return 0;
}

int jvt_net_reactor_init(jvt_net_reactor_t *reactor)
{
	assert(reactor);
	int ret = jvt_netio_init(&reactor->net_io, 4096, -1);
	if ( 0 != ret)
	{
		UDP_ERR("failed to initalize net io! ret=%d", ret);
		return -1;
	}

	reactor->is_started = 1;

    ret = pthread_create(&reactor->tid, NULL, thread_proc, reactor);
	if ( 0 != ret)
	{
		UDP_ERR("failed to create thread! ret=%d", ret);
		return -2;
	}

	return 0;
}

void jvt_net_reactor_uninit(jvt_net_reactor_t *reactor)
{
	jvt_netio_uninit(&reactor->net_io);
}

int jvt_net_reactor_watch(jvt_net_reactor_t *reactor)
{
	int idxs = -1;
	int i;

	while (reactor->is_started)
	{
		idxs = jvt_netio_wait(&reactor->net_io);
		if(idxs < 0)
		{
			if(errno == EINTR)
				continue;

			LOG_ERR("failed to watch i/o! err: %s", strerror(errno));
			break;
		}

		for(i = 0; i < idxs; ++i)
		{
			udp_socket_t* udp_socket = (udp_socket_t*)jvt_netio_get_key(&reactor->net_io, i);
			int events = jvt_netio_get_event(&reactor->net_io, i);
			if(events & EPOLLIN)
			{
				_on_recv(reactor, udp_socket);
			}

			if(events & EPOLLOUT)
			{
				_on_send(reactor, udp_socket);
			}
		}
	}

	return 0;
}

int jvt_net_reactor_run(jvt_net_reactor_t *reactor)
{
	//TODO: 
	// 此时应该循环从队列（reactor->recv_events）中取出事件，并分发到上层会话session进行处理
	// ***

	// 临时处理: 阻塞主线�?
	int ret = pthread_join(reactor->tid, NULL);
	if (ret != 0)
	{
		UDP_ERR("failed to join thread!");
		return -1;
	}

	return 0;
}

int jvt_net_reactor_add_event(jvt_net_reactor_t *reactor, udp_socket_t *sock, int events)
{
	return jvt_netio_add_fd(&reactor->net_io, sock->info->sockfd, events, sock);
}

int jvt_net_reactor_del_event(jvt_net_reactor_t *reactor, udp_socket_t *sock, int events)
{
	return jvt_netio_del_fd(&reactor->net_io, sock->info->sockfd, events);
}

int jvt_net_reactor_mod_event(jvt_net_reactor_t *reactor, udp_socket_t *sock, int events)
{
	return jvt_netio_modify_fd(&reactor->net_io, sock->info->sockfd, events, sock);
}

int jvt_net_reactor_push_event(jvt_net_reactor_t *reactor, udp_socket_t *sock, jvt_net_event_t* ev)
{
	// TODO: 
	// que_of_event_.push(ev);

	// 临时处理
	return udp_socket_send(sock, ev->data, ev->len);
}