
#ifndef EVENT_HANDLER_H_
#define EVENT_HANDLER_H_

#include "event.h"
#include "eventtype.h"

class IEventHandler
{
public:
    IEventHandler() {}
    virtual ~IEventHandler() {}
    virtual IEvent* handle(const IEvent* ev) { return NULL; }
};

#endif//EVENT_HANDLER_H_

