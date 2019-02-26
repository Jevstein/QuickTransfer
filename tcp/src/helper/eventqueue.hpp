/*
 *  EventQueue.hpp 
 *  YiEventQueue
 *
 *  Created by Jevstein on 2018/4/9 20:14.
 *  Copyright @ 2018year Jevstein. All rights reserved.
 *
 */
#ifndef _EVENT_QUEUE_H_
#define _EVENT_QUEUE_H_
#include <queue>
#include "locker.hpp"

template<class T, class LOCKER = CLocker>
class CEventQueue
{
public:
	CEventQueue(void){}
	~CEventQueue(void){}

public:
	bool push(T ev){
		CLockGuard<LOCKER> lock(locker_);
		que_of_event_.push(ev);
		return true;
	}

	size_t size(){
		CLockGuard<LOCKER> lock(locker_);
		return que_of_event_.size();
	}

	bool empty(){
		CLockGuard<LOCKER> lock(locker_);
		return que_of_event_.empty();
	}

	T front(){
		T ev = 0;
		if (!que_of_event_.empty())
		{
			CLockGuard<LOCKER> lock(locker_);
			if (!que_of_event_.empty())
			{
				ev = que_of_event_.front();
			}
		}
		return ev;
	}

	T pop(){
		T ev = 0;
		if (!que_of_event_.empty())
		{
			CLockGuard<LOCKER> lock(locker_);
			if (!que_of_event_.empty())
			{
				ev = que_of_event_.front();
				que_of_event_.pop();
			}
		}
		return ev;
	}

private:
	std::queue<T> que_of_event_;
	LOCKER locker_;
};

#endif//_YI_EVENT_QUEUE_H_
