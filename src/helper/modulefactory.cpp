#include "prefixhead.h"
#include "modulefactory.h"

CModuleFactory::CModuleFactory(const char* module_file)
: handler_(NULL)
{
	if (module_file != NULL)
	{
		init(module_file);
	}
}

bool CModuleFactory::init(const char* module_file)
{
	handler_ = DYNLIB_LOAD(module_file);
	if (handler_ == 0)
	{
		LOG_ERR("failed to dlopen '%s'! error code: '%s'", module_file, p_last_error());
		return false;
	}
	return true;
}

CModuleFactory::~CModuleFactory(void)
{
	if (handler_ != 0)
	{
		DYNLIB_UNLOAD(handler_);
	}
}

void* CModuleFactory::loadfunc(const char* func_name)
{
	if (handler_ == 0)
	{
		LOG_ERR("failed to load '%s', error code: handler_ is null", func_name);
		return NULL;
	}

	void *pfn = DYNLIB_GETSYM(handler_, func_name);
	if (!pfn)
	{
		LOG_ERR("failed to dlsym '%s'! error code: '%s'", func_name, p_last_error());
		return NULL;
	}

	return pfn;
}
