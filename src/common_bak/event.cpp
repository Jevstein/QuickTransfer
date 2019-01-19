#include "../server/prefixhead.h"
#include "event.h"

IEvent::IEvent(u32 eid, u32 sn) : eid_(eid), sn_(sn)
{
}

IEvent::~IEvent()
{
}

u32 IEvent::generateSeqNo()
{
    static u32 sn = 0;
	return sn++;
}
