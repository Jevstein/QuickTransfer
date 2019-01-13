/*
 *  NetEvent.h 
 *  netevent
 *
 *  Created by Jevstein on 2018/4/12 19:04.
 *  Copyright @ 2018year Jevstein. All rights reserved.
 *
 */
#ifndef _NET_EVENT_H_
#define _NET_EVENT_H_

class CConnection;

/**CTCPEvent*/
class CTCPEvent: public IEvent
{
public:
	CTCPEvent();
	virtual ~CTCPEvent(void);

public:
	virtual void process();
	virtual void release();
    
    virtual void set(CConnection* connection, const char* packet, int packet_len);

public:
	char* get_data();
	int get_length() { return packet_len_ - offset_; }
	void transfer_data(int transfer_num);
	uint get_delay() { return delay_; }
	void set_delay(uint time) { delay_ = time; }

private:
	char* packet_buffer_;
	int32_t packet_len_;
	int32_t offset_;
	uint delay_;
	CConnection* connection_;
    CLocker locker_;
};


#endif//_NET_EVENT_H_
