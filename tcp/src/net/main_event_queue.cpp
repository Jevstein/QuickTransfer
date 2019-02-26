#include "prefixhead.h"
#include "main_event_queue.h"

CMainEventQueue::CMainEventQueue(void)
{
}

CMainEventQueue::~CMainEventQueue(void)
{

}

void CMainEventQueue::push(IEvent *ev)
{
	que_of_main_event_.push(ev);
}

IEvent* CMainEventQueue::pop()
{
	return que_of_main_event_.pop();
}

int CMainEventQueue::size()
{
	return que_of_main_event_.size();
}
