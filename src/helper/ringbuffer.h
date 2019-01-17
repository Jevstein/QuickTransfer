/*
 *  ringbuffer.h 
 *  thread
 *
 *  Created by Jevstein on 2019/1/17 7:45.
 *  Copyright @ 2019year Jevstein. All rights reserved.
 *
 */
#ifndef _RING_BUFFER_H_
#define _RING_BUFFER_H_
#include "globaldefine.h"

class CRingBuffer
{
public:
	CRingBuffer(uint capacity);
	~CRingBuffer();

public:
	// char* get_buffer();

protected:
	char* buffer_;
	uint capacity_;
	uint size_;
	uint begin_pos_;
	uint end_pos_;
};
#endif//_RING_BUFFER_H_
