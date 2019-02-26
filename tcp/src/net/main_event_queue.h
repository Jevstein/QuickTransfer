/*
 *  MainEventQueue.h 
 *  MainEventQueue
 *
 *  Created by Jevstein on 2018/4/10 19:52.
 *  Copyright @ 2018year Jevstein. All rights reserved.
 *
 */
#ifndef _MAIN_EVENT_QUEUE_H_
#define _MAIN_EVENT_QUEUE_H_

class CMainEventQueue
{
public:
	CMainEventQueue(void);
	~CMainEventQueue(void);

public:
	void push(IEvent *ev);
	IEvent* pop();
	int size();

private:
	CEventQueue<IEvent*> que_of_main_event_;
};

#endif//_MAIN_EVENT_QUEUE_H_
