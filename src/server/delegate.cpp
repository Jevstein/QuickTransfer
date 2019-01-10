#include "prefixhead.h"
#include "delegate.h"

CDelegate::CDelegate()
: started_(true)
{
}

CDelegate::~CDelegate()
{

}

bool CDelegate::init()
{
	return true;
}

void CDelegate::run()
{

}

void CDelegate::stop()
{
	started_ = false;
}
