/*
 *  IYiEvent.h 
 *  IYiEvent
 *
 *  Created by Jevstein on 2018/4/9 20:10.
 *  Copyright @ 2018year Jevstein. All rights reserved.
 *
 */
#ifndef _EVENT_H_
#define _EVENT_H_

class IEvent
{
public:
	virtual ~IEvent(){}
	virtual void process() = 0;
	virtual void release() = 0;
};
#endif//_EVENT_H_
