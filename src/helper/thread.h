/*
 *  thread.h 
 *  thread
 *
 *  Created by Jevstein on 2018/4/10 12:09.
 *  Copyright @ 2018year Jevstein. All rights reserved.
 *
 */
#ifndef _THREAD_H_
#define _THREAD_H_
#include "platform.h"

class CThread
{
	static p_thread_retval_t P_THREAD_PROC proc(void* arg);
public:
	CThread();
	virtual ~CThread();

public:
	int start();
	void stop();

protected:
	virtual void run() = 0;

protected:
	bool is_started_;
	p_thread_t thread_handle_;
};
#endif//_THREAD_H_
