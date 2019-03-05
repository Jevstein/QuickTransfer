#include "inner.h"
#include "net_io.h"

#ifdef JVT_OS_MAC

int jvt_netio_init(jvt_net_io_t *net_io, int fds/* = 4096*/, int model/* = -1*/)
{
    assert(net_io);

    net_io->fd = kqueue();
    if (-1 == net_io->fd)
        return -1;
    
	net_io->model = IO_KQUEUE;//model;
	net_io->events = calloc(fds, sizeof(struct kevent));
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
	int i = 0;
    struct kevent changelist[2];
    
	if (events & EPOLLIN)
        EV_SET(&changelist[i++], sockfd, EVFILT_READ, EV_ADD | EV_CLEAR, 0, 0, (void*)(intptr_t)key);
    
	if (events & EPOLLOUT)
        EV_SET(&changelist[i++], sockfd, EVFILT_WRITE, EV_ADD | EV_CLEAR, 0, 0, (void*)(intptr_t)key);

	return kevent(net_io->fd, changelist, i, NULL, 0, NULL);
}

int jvt_netio_del_fd(jvt_net_io_t *net_io, int sockfd, int events)
{
	int i = 0;
    struct kevent changelist[2];
    
	if (events == 0)
    {
		events = EPOLLIN | EPOLLOUT;
	}
    
	if (events & EPOLLIN)
        EV_SET(&changelist[i++], sockfd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
    
	if (events & EPOLLOUT)
        EV_SET(&changelist[i++], sockfd, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);

	return kevent(net_io->fd, changelist, i, NULL, 0, NULL);
}

int jvt_netio_modify_fd(jvt_net_io_t *net_io, int sockfd, int events, void* key)
{
	jvt_netio_del_fd(net_io, sockfd, events);
	return jvt_netio_add_fd(net_io, sockfd, events, key);
}

int jvt_netio_wait(jvt_net_io_t *net_io)
{
	int waitms = 200;
	struct timespec timeout;
	timeout.tv_sec = waitms / 1000;
	timeout.tv_nsec = (waitms % 1000) * 1000 * 1000;

	return kevent(net_io->fd, NULL, 0, net_io->events, net_io->max_events, &timeout);
}

void jvt_netio_close(jvt_net_io_t *net_io)
{
	close(net_io->fd);
	net_io->fd = -1;
}

int jvt_netio_get_event(jvt_net_io_t *net_io, int idx)
{
	return ((struct kevent *)net_io->events)[idx].filter;
}

void* jvt_netio_get_key(jvt_net_io_t *net_io, int idx)
{
	return ((struct kevent *)net_io->events)[idx].udata;
}

#endif//JVT_OS_MAC