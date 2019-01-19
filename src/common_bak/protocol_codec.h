
#ifndef BRKS_INTF_PROTOCOL_CODEC_H_
#define BRKS_INTF_PROTOCOL_CODEC_H_

#include "glo_def.h"
#include "eventtype.h"
#include "user_events.h"

class protocol_codec_t
{
public:
    virtual bool encode(IEvent* ev, u8* buffer, u32 size) = 0;
    virtual IEvent* decode(u16 mid, u8* buffer, u32 size) = 0;
};

#endif

