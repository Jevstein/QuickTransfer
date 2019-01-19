
#ifndef NS_EVENT_H_
#define NS_EVENT_H_

#include "glo_def.h"
#include <string>


class IEvent
{
public:
	IEvent(u32 eid, u32 sn);
	virtual ~IEvent();
    virtual std::ostream& dump(std::ostream& out) const { return out; }; 
    u32 generateSeqNo();
    u32 get_eid() const { return eid_; };
    u32 get_sn() const { return sn_; };
    void set_eid(u32 eid) { eid_ = eid; };
    
private:
    u32   eid_;    /* �¼�ID */
    u32   sn_;     /* �¼������к� */
};

#endif//NS_EVENT_H_

