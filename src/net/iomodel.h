/*
 *  IOModel.h 
 *  IOModel
 *
 *  Created by Jevstein on 2018/6/19 10:20.
 *  Copyright @ 2018year Jevstein. All rights reserved.
 *
 */
#ifndef _IO_MODEL_H_
#define _IO_MODEL_H_

class CIOModel
{
public:
	CIOModel();
	~CIOModel(void);

public:
	int init(int nfds = 4096);
	bool addfd(YI_SOCKET sockfd, int events, void* key);
	bool deletefd(YI_SOCKET sockfd, int events = 0);
	bool modifyfd(YI_SOCKET sockfd, int events, void* key);
	int wait();
	void close();

	int get_event(int idx);
	void* get_userdata(int idx);

protected:
	YI_EPFD epoll_fd_;
	net_io_event* ioevents_;
	int max_events_;
};

#endif//_IO_MODEL_H_
