/*
 *  ringbuffer.h 
 *  thread
 *
 *  Created by Jevstein on 2019/1/17 7:45.
 *  Copyright @ 2019year Jevstein. All rights reserved.
 *
 */
#ifndef _RECV_BUFFER_H_
#define _RECV_BUFFER_H_

class CRecvBuffer
{
public:
	CRecvBuffer(uint capacity);
	~CRecvBuffer();

public:
	void reset();
	char* get_wdata(uint length, bool *rw);
	void rewrite_data(char* data, uint length);

private:
	char temp_buffer_[4096];
	char* buffer_;
	uint capacity_;
	uint size_;
	uint begin_pos_;
	uint end_pos_;
};
#endif//_RING_BUFFER_H_
