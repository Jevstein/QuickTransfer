#include "prefixhead.h"
#include "io_model.h"
#include "io_epoll.h"
#include "io_kqueue.h"

CIOModel::CIOModel(int type)
: fd_(-1)
, model_type_(type)
{

}

CIOModel::~CIOModel()
{
}

CIOModel *CIOModel::create_model(int type /*= 0*/)
{
#if defined(YI_OS_LINUX)//linux: epoll
	return new CIOEpoll(IO_EPOLL);
#elif defined(YI_OS_MAC)//mac: kqueue
	return new CIOKqueue(IO_KQUEUE);
#elif defined(YI_OS_WIN32)//windows: select
	return new CIOSelect(IO_SELECT);
#else
	return NULL;
#endif
}

void CIOModel::destry_model(CIOModel *mode)
{
	if (mode)
		delete mode;
}