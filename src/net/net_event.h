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

/**CNetPacket*/
class CNetPacket: public IPacket
{
public:
	CNetPacket(): data_(NULL), length_(0), offset_(0){}
    virtual ~CNetPacket() { clear(); }
    virtual const char* data() const { return data_; }
    virtual int length() const { return length_; }

public:
    char* get_data() { return (data_ ? (data_ + offset_) : NULL); }
    int get_length() const { return length_ + offset_; }
	bool is_empty() const { return ((NULL == data_) && (0 == length_) && (0 == offset_)); }

	bool add_offset(int len) { offset_ += len; return (offset_ < length_); }
	bool set_data(const char* data, int len);
	void clear();

private:
	char *data_;
	int32_t length_;
	int32_t offset_;
};

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
	char* get_data() { return packet_.get_data(); }
	int get_length() { return packet_.get_length(); }
	void transfer_data(int len) { packet_.add_offset(len); }
	uint get_delay() { return delay_; }
	void set_delay(uint time) { delay_ = time; }

private:
	// char* packet_buffer_;
	// int32_t packet_len_;
	// int32_t offset_;
	CNetPacket packet_;
	uint delay_;
	CConnection* connection_;

    CLocker locker_;
};


#endif//_NET_EVENT_H_
