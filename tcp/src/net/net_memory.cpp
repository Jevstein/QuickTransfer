#include "prefixhead.h"
#include "net_memory.h"

CNetMemory::CNetMemory(void)
{
}

CNetMemory::~CNetMemory(void)
{
}

void* CNetMemory::Malloc(size_t length)
{
    // TODO: to use memory pool!!!
    // ***

//	CLockGuard<CLocker> lock(locker_);
    void *p = (void *) new char[length];
    if (!p)
        LOG_ERR("failed to malloc!");
    
    return p;
}

void CNetMemory::Free(char *data)
{
	if (NULL == data)
		return;

    // TODO: to use memory pool!!!
    // ***

//	CLockGuard<CLocker> lock(locker_);
    delete []data;
}
