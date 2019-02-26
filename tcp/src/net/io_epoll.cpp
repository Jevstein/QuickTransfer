#include "prefixhead.h"
#include "io_epoll.h"

#ifdef YI_OS_LINUX

CIOEpoll::CIOEpoll(int type)
: CIOModel(type)
, events_(NULL)
, max_events_(0)
{

}

CIOEpoll::~CIOEpoll(void)
{
	if (fd_ != -1)
	{
		CLOSE_SOCKET(fd_);
		fd_ = -1;
	}

	if (events_ != NULL)
	{
		delete[] events_;
		events_ = NULL;
	}
}

int CIOEpoll::init(int fds /*= 4096*/)
{
	if (-1 != (fd_ = epoll_create(fds)))
	{
		events_ = new epoll_event[fds];
		max_events_ = fds;
	}
	return fd_;
}

int CIOEpoll::add_fd(int sockfd, int events, void* key)
{
	return epoll_ctl_fd(EPOLL_CTL_ADD, sockfd, events, key);
}

int CIOEpoll::del_fd(int sockfd, int events /*= 0*/)
{
	return epoll_ctl_fd(EPOLL_CTL_DEL, sockfd, events, NULL);
}

int CIOEpoll::modify_fd(int sockfd, int events, void* key)
{
	return epoll_ctl_fd(EPOLL_CTL_MOD, sockfd, events, key);
}

int CIOEpoll::wait()
{
	int nfds = epoll_wait(fd_, events_, max_events_, 200);
	return nfds;
}

void CIOEpoll::close()
{
	CLOSE_SOCKET(fd_);
	fd_ = -1;
}

int CIOEpoll::epoll_ctl_fd(int op, int sockfd, int events, void* key)
{
	struct epoll_event plevt;
	plevt.events = events;
	plevt.data.ptr = key;

	return epoll_ctl(fd_, op, sockfd, &plevt);
}

int CIOEpoll::get_event(int idx)
{
	return events_[idx].events;
}

void* CIOEpoll::get_key(int idx)
{
	return events_[idx].data.ptr; 
}


#endif //YI_OS_LINUX
