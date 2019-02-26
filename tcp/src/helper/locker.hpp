/*
 *  locker.hpp
 *  locker
 *
 *  Created by Jevstein on 2018/3/27.
 *  Copyright @2018 Jevstein. All rights reserved.
 *
 */
#ifndef _LOCKER_H_
#define _LOCKER_H_
#include "platform.h"

/** CLocker*/
class CLocker
{
public:
	CLocker() { p_mutex_init(&mutex_); }
	~CLocker() { p_mutex_destroy(&mutex_); }
	
public:
	void enter() { p_mutex_lock(&mutex_); }
	void leave() { p_mutex_unlock(&mutex_); }
	//int try_enter();
	
private:
	p_mutex_t mutex_;
};

/** CEmptyLocker*/
class CEmptyLocker
{
public:
	CEmptyLocker(void) {}
	~CEmptyLocker(void) {}
	
public:
	void enter() {}
	void leave() {}
};

/** CLockGuard*/
template<class T>
class CLockGuard
{
public:
	CLockGuard(T& locker) : locker_(locker) { locker_.enter(); }
	~CLockGuard() { locker_.leave(); }

private:
	T& locker_;
};
#endif//_LOCKER_H_
