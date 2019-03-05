#include "inner.h"
#include "net_reactor.h"



int jvt_net_reactor_init(jvt_net_reactor_t *reactor)
{
	assert(reactor);
	if ( 0 != jvt_netio_init(&reactor->net_io, 4096, -1))
		return -1;

	return 0;
}

void jvt_net_reactor_uninit(jvt_net_reactor_t *reactor)
{
	jvt_netio_uninit(&reactor->net_io);
}

int jvt_net_reactor_run(jvt_net_reactor_t *reactor)
{
	int idxs = -1;
	int i;

	while (1)
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
			udp_socket_t* udp_socket = jvt_netio_get_key(&reactor->net_io, i);
			int events = jvt_netio_get_event(&reactor->net_io, i);
			if(events & EPOLLIN)
			{
				udp_socket_recv(udp_socket);
			}

			if(events & EPOLLOUT)
			{
			}
		}
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
