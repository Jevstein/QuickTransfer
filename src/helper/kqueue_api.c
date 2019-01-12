/*
 *  kqueue_api.cpp 
 *  kqueue_api
 *
 *  Created by Jevstein on 2018/11/1 17:06.
 *  Copyright @ 2018year Jevstein. All rights reserved.
 *
 */
#include "prefixhead.h"
#include "epoll_api.h"

#ifdef YI_OS_MAC
#include <sys/event.h>

YI_EPFD io_create(net_io_event** events, int nfds /*= 4096*/)
{
	YI_EPFD epfd = kqueue();
	if (epfd < 0)
	{
		LOG_ERR("failed to io_create! epfd: %d, err: %s", epfd, strerror(errno));
		return -1;
	}
	
	// events = new struct kevent[fds];
	*events = (struct kevent *)malloc(sizeof(struct kevent) * nfds);
	if (NULL == *events)
	{
		LOG_ERR("failed to new epoll_event!");
		return -2;
	}

	LOG_INF("create epoll success! nfds: %d", nfds);

	return epfd;
}

bool io_add_fd(YI_EPFD epfd, YI_SOCKET sockfd, int events, void* key)
{
	int ret = 0;
	int i = 0;
    struct kevent changelist[2];
    
	if (events & EPOLLIN)
        EV_SET(&changelist[i++], sockfd, EVFILT_READ, EV_ADD | EV_CLEAR, 0, 0, (void*)(intptr_t)key);
    
	if (events & EPOLLOUT)
        EV_SET(&changelist[i++], sockfd, EVFILT_WRITE, EV_ADD | EV_CLEAR, 0, 0, (void*)(intptr_t)key);

	ret = kevent(epfd, changelist, i, NULL, 0, NULL);
	if (0 != ret)
	{
		LOG_ERR("failed to set EPOLL_CTL_ADD! events: %d, ret: %d, err: %s", events, ret, strerror(errno));
		return false;
	}

	LOG_INF("set EPOLL_CTL_ADD success! sockfd=%d, events=%d", sockfd, events);
	return true;
}

bool io_modify_fd(YI_EPFD epfd, YI_SOCKET sockfd, int events, void* key)
{
	io_del_fd(epfd, sockfd, events);
	return io_add_fd(epfd, sockfd, events, key);
}

bool io_del_fd(YI_EPFD epfd, YI_SOCKET sockfd, int events /*= 0*/)
{
	int ret = 0;
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

	ret = kevent(epfd, changelist, i, NULL, 0, NULL);
	if (0 != ret)
	{
		LOG_ERR("failed to set EPOLL_CTL_DEL! events: %d, ret: %d, err: %s", events, ret, strerror(errno));
		return false;
	}

	LOG_INF("set EPOLL_CTL_DEL success! sockfd=%d, events=%d", sockfd, events);

	return true;
}

int io_wait(YI_EPFD epfd, net_io_event** events, int maxevents/* = 2000*/)
{
	int waitms = 200;
	struct timespec timeout;
	timeout.tv_sec = waitms / 1000;
	timeout.tv_nsec = (waitms % 1000) * 1000 * 1000;

	return kevent(epfd, NULL, 0, (struct kevent *)*events, maxevents, &timeout);
}

void io_close(YI_EPFD epfd, net_io_event* events)
{
	close(epfd);
	// delete[] events;s
	free(events);

	LOG_INF("close kqueue success!");
}

int io_get_event(net_io_event** events, int idx)
{
	return (*(struct kevent **)events)[idx].filter;;
}

void* io_get_userdata(net_io_event** events, int idx)
{
	return (*(struct kevent **)events)[idx].udata;
}

#endif //YI_OS_MAC