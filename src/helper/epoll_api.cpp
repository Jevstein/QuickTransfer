/*
 *  epoll_api.cpp 
 *  epoll_api
 *
 *  Created by Jevstein on 2018/11/1 17:06.
 *  Copyright @ 2018year Jevstein. All rights reserved.
 *
 */
#include "prefixhead.h"
#include "epoll_api.h"

#ifdef YI_OS_LINUX
#include <sys/epoll.h>

int epoll_ctl_fd(YI_EPFD epfd, int op, YI_SOCKET sockfd, int events, void* key)
{
	struct epoll_event plevt;
	plevt.events = events;
	plevt.data.ptr = key;

	return ::epoll_ctl(epfd, op, sockfd, &plevt);
}

YI_EPFD io_create(net_io_event** events, int nfds /*= 4096*/)
{
	YI_EPFD epfd = ::epoll_create(nfds);
	if (epfd < 0)
	{
		LOG_ERR("failed to epoll_create! epfd: %d, err: %s", epfd, strerror(errno));
		return -1;
	}

	*events = new epoll_event[nfds];
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
	int ret = epoll_ctl_fd(epfd, EPOLL_CTL_ADD, sockfd, events, key);
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
	int ret = epoll_ctl_fd(epfd, EPOLL_CTL_MOD, sockfd, events, key);
	if (0 != ret)
	{
		LOG_ERR("failed to set EPOLL_CTL_MOD! events: %d, ret: %d, err: %s", events, ret, strerror(errno));
		return false;
	}

	LOG_INF("set EPOLL_CTL_MOD success! sockfd=%d, events=%d", sockfd, events);

	return true;
}

bool io_del_fd(YI_EPFD epfd, YI_SOCKET sockfd, int events /*= 0*/)
{
	int ret = epoll_ctl_fd(epfd, EPOLL_CTL_DEL, sockfd, events, NULL);
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
	return ::epoll_wait(epfd, (struct epoll_event *)*events, maxevents, 200);
}

void io_close(YI_EPFD epfd, net_io_event* events)
{
	::close(epfd);
	delete[] events;

	LOG_INF("close epoll success!");
}

int io_get_event(net_io_event** events, int idx)
{
	return (*(struct epoll_event **)events)[idx].events;
}

void* io_get_userdata(net_io_event** events, int idx)
{
	return (*(struct epoll_event **)events)[idx].data.ptr;
}

#endif //YI_OS_LINUX