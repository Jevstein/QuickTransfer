/*
 *  IOModelKqueue.h 
 *  kqueue
 *
 *  Created by Jevstein on 2018/4/12 18:39.
 *  Copyright @ 2018year Jevstein. All rights reserved.
 *
 */
#ifndef _IO_KQUEUE_H_
#define _IO_KQUEUE_H_
#ifdef YI_OS_MAC
#include <sys/event.h>
#include "io_model.h"

class CIOKqueue : public CIOModel
{
public:
	CIOKqueue(int type);
	virtual ~CIOKqueue();
	virtual int init(int nfds = 4096);
	virtual int add_fd(int sockfd, int events, void* key);
	virtual int del_fd(int sockfd, int events = 0);
	virtual int modify_fd(int sockfd, int events, void* key);
	virtual int wait();
	virtual void close();

	virtual int get_event(int idx);
	virtual void* get_key(int idx);

private:
	struct kevent* events_;
	int max_events_;
};

#endif//YI_OS_MAC
#endif//_IO_KQUEUE_H_
