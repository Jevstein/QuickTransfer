#include "inner.h"
#include "net_io.h"

#ifdef JVT_OS_LINUX

int _epoll_ctl_fd(jvt_net_io_t *net_io, int op, int sockfd, int events, void* key)
{
	struct epoll_event plevt;
	plevt.events = events;
	plevt.data.ptr = key;

	return epoll_ctl(net_io->fd, op, sockfd, &plevt);
}

int jvt_netio_init(jvt_net_io_t *net_io, int fds/* = 4096*/, int model/* = -1*/)
{
    assert(net_io);

    net_io->fd = epoll_create(fds);
    if (-1 == net_io->fd)
        return -1;
    
	net_io->model = IO_EPOLL;//model;
	net_io->events = calloc(fds, sizeof(struct epoll_event));
	net_io->max_events = fds;

    return 0;
}

void jvt_netio_uninit(jvt_net_io_t *net_io)
{
    if (net_io->fd != -1)
	{
		close(net_io->fd);
		net_io->fd = -1;
	}

	if (net_io->events != NULL)
	{
		free(net_io->events);
		net_io->events = NULL;
	}
}

int jvt_netio_add_fd(jvt_net_io_t *net_io, int sockfd, int events, void* key)
{
    return _epoll_ctl_fd(net_io, EPOLL_CTL_ADD, sockfd, events, key);
}

int jvt_netio_del_fd(jvt_net_io_t *net_io, int sockfd, int events)
{
	return _epoll_ctl_fd(net_io, EPOLL_CTL_DEL, sockfd, events, NULL);
}

int jvt_netio_modify_fd(jvt_net_io_t *net_io, int sockfd, int events, void* key)
{
	return _epoll_ctl_fd(net_io, EPOLL_CTL_MOD, sockfd, events, key);
}

int jvt_netio_wait(jvt_net_io_t *net_io)
{
    return epoll_wait(net_io->fd, (struct epoll_event *)net_io->events, net_io->max_events, 200);
}

void jvt_netio_close(jvt_net_io_t *net_io)
{
	close(net_io->fd);
	net_io->fd = -1;
}

int jvt_netio_get_event(jvt_net_io_t *net_io, int idx)
{
	return ((struct epoll_event *)net_io->events)[idx].events;
}

void* jvt_netio_get_key(jvt_net_io_t *net_io, int idx)
{
	return ((struct epoll_event *)net_io->events)[idx].data.ptr;
}

#endif//JVT_OS_LINUX