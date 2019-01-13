#include "prefixhead.h"
#include "thread.h"

CThread::CThread()
: is_started_(false)
, thread_handle_(0)
{
}

CThread::~CThread()
{
}

int CThread::start()
{
    LOG_DBG("CThread::thread[%p] start ...", this);

	is_started_ = true;
	int ret = p_thread_create(&thread_handle_, proc, (p_thread_arg_t)this);
	if (ret != 0)
	{
		is_started_ = false;
		LOG_ERR("failed to create thread!");
	}
	return ret;
}

void CThread::stop()
{
	if (is_started_)
	{
		LOG_DBG("CThread::thread[%p] stop ...", this);

		is_started_ = false;
		p_thread_wait_exit(&thread_handle_);
	}
}

p_thread_retval_t CThread::proc(void* arg)
{
	CThread* pthis = reinterpret_cast<CThread*>(arg);
	if (pthis)
	{
		pthis->run();
		pthis->stop();
	}

	return 0;
}
