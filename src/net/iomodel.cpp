#include "prefixhead.h"
#include "iomodel.h"

CIOModel::CIOModel()
: epoll_fd_(0)
, ioevents_(NULL)
, max_events_(0)
{

}

CIOModel::~CIOModel()
{
}

int CIOModel::init(int nfds/* = 4096*/)
{
	max_events_ = nfds;
	epoll_fd_ = io_create(&ioevents_, nfds);
	return epoll_fd_;
}

void CIOModel::close()
{
	io_close(epoll_fd_, ioevents_);
}

bool CIOModel::addfd(YI_SOCKET sockfd, int events, void* key)
{
	return io_add_fd(epoll_fd_, sockfd, events, key);
}

bool CIOModel::deletefd(YI_SOCKET sockfd, int events /*= 0*/)
{
	return io_del_fd(epoll_fd_, sockfd, events);
}

bool CIOModel::modifyfd(YI_SOCKET sockfd, int events, void* key)
{
	return io_modify_fd(epoll_fd_, sockfd, events, key);
}

int CIOModel::wait()
{
	return io_wait(epoll_fd_, &ioevents_, max_events_);
}

int CIOModel::get_event(int idx)
{
	return io_get_event(&ioevents_, idx);
}

void* CIOModel::get_userdata(int idx)
{
	return io_get_userdata(&ioevents_, idx);
}
