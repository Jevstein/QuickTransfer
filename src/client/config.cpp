#include "prefixhead.h"
#include "config.h"
#include "delegate.h"

CConfig::CConfig()
{
}

CConfig::~CConfig()
{
}

void CConfig::cleanup()
{
}

void CConfig::init()
{
    cfg_info_.ip = "127.0.0.1";
	cfg_info_.port = 6666;
}