/*
 *  IOModelEpoll.h 
 *  epoll
 *
 *  Created by Jevstein on 2018/4/12 18:24.
 *  Copyright @ 2018year Jevstein. All rights reserved.
 *
 */
#ifndef _IO_EPOLL_H_
#define _IO_EPOLL_H_
#ifdef YI_OS_LINUX
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "io_model.h"

class CIOEpoll : public CIOModel
{
public:
	CIOEpoll(int type);
	virtual ~CIOEpoll();

public:
	virtual int init(int nfds = 4096);
	virtual int add_fd(int sockfd, int events, void* key);
	virtual int del_fd(int sockfd, int events = 0);
	virtual int modify_fd(int sockfd, int events, void* key);
	virtual int wait();
	virtual void close();

	virtual int get_event(int idx);
	virtual void* get_key(int idx);

protected:
	int epoll_ctl_fd(int op, int sockfd, int events, void* key);

private:
	struct epoll_event* events_;
	int max_events_;
};

#endif //YI_OS_LINUX
#endif//_IO_EPOLL_H_
