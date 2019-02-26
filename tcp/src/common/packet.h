
#ifndef _PACKET_H_
#define _PACKET_H_

#include "glo_def.h"
#include <string>


class CPacket: public IPacket
{
public:
	CPacket(u32 eid, u32 sn): eid_(eid), sn_(sn) {}
	virtual ~CPacket() {}
    virtual const char* data() const { return (char *)this; }
    virtual int length() const { return sizeof(CPacket); }

    virtual std::ostream& dump(std::ostream& out) const { return out; }

public:
    u32 generate_sn() { static u32 sn = 0; return sn++; }
    u32 get_eid() const { return eid_; }
    u32 get_sn() const { return sn_; }
    void set_eid(u32 eid) { eid_ = eid; }
    
private:
    u32 eid_;
    u32 sn_;
};

#endif//_PACKET_H_

