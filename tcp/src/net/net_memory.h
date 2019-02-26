/*
 *  NetMemory.h
 *  NetMemory
 *
 *  Created by Jevstein on 2018/4/12 19:46.
 *  Copyright @ 2018year Jevstein. All rights reserved.
 *
 *  use memory pool
 */

#ifndef _NET_MEMORY_H_
#define _NET_MEMORY_H_

class CNetMemory
{
public:
	CNetMemory(void);
	virtual ~CNetMemory(void);

public:
	void *Malloc(size_t length);
	void Free(char *data);

// private:
// 	CLocker locker_;
};

#endif //_NET_MEMORY_H_
