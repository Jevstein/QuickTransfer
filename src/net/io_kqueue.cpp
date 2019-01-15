#include "prefixhead.h"
#include "io_kqueue.h"

#ifdef YI_OS_MAC

CIOKqueue::CIOKqueue(int type)
: CIOModel(type)
, events_(NULL)
, max_events_(0)
{

}

CIOKqueue::~CIOKqueue(void)
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

int CIOKqueue::init(int fds /*= 4096*/)
{
	if (-1 != (fd_ = kqueue()))
	{
		max_events_ = fds;
		events_ = new struct kevent[fds];
	}

	return fd_;
}

int CIOKqueue::add_fd(int fd, int events, void* key)
{
	int i = 0;
    struct kevent changelist[2];
    
	if (events & EPOLLIN)
        EV_SET(&changelist[i++], fd, EVFILT_READ, EV_ADD | EV_CLEAR, 0, 0, (void*)(intptr_t)key);
    
	if (events & EPOLLOUT)
        EV_SET(&changelist[i++], fd, EVFILT_WRITE, EV_ADD | EV_CLEAR, 0, 0, (void*)(intptr_t)key);

	return kevent(fd_, changelist, i, NULL, 0, NULL);
}

int CIOKqueue::del_fd(int fd, int events /*= 0*/)
{
    int i = 0;
    struct kevent changelist[2];
    
	if (events == 0)
    {
		events = EPOLLIN | EPOLLOUT;
	}
    
	if (events & EPOLLIN)
        EV_SET(&changelist[i++], fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
    
	if (events & EPOLLOUT)
        EV_SET(&changelist[i++], fd, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);

	return kevent(fd_, changelist, i, NULL, 0, NULL);
}

int CIOKqueue::modify_fd(int fd, int events, void* key)
{
	del_fd(fd, events);
	return add_fd(fd, events, key);
}

int CIOKqueue::wait()
{
	int waitms = 200;
	struct timespec timeout;
	timeout.tv_sec = waitms / 1000;
	timeout.tv_nsec = (waitms % 1000) * 1000 * 1000;

	int nfds = kevent(fd_, NULL, 0, events_, max_events_, &timeout);
	return nfds;
}

void CIOKqueue::close()
{
	CLOSE_SOCKET(fd_);
	fd_ = -1;
}

int CIOKqueue::get_event(int idx)
{
	return events_[idx].filter; 
}

void* CIOKqueue::get_key(int idx) 
{
	return events_[idx].udata;
}

#endif //YI_OS_MAC
