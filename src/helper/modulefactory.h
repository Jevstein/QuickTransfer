/*
 *  ModuleFactory.h
 *  ModuleFactory
 *
 *  Created by Jevstein on 2018/3/28 19:18.
 *  Copyright @ 2018year Jevstein. All rights reserved.
 *
 */
#ifndef _MODULEFACTORY_H_
#define _MODULEFACTORY_H_
#include <stdio.h>
#include "platform.h"
#include <dlfcn.h>

class CModuleFactory
{
public:
	CModuleFactory(const char* module_file);
	~CModuleFactory(void);

public:
	template<class T>
	T* createmodule(const char* func_name)
	{
		typedef	T* (*PFUNC)();
		PFUNC pfn = 0;
		T* func = NULL;
		
		pfn = (PFUNC)loadfunc(func_name);
		if (pfn)
		{
			func = pfn();
		}
		return func;
	}

protected:
	bool init(const char* module_file);
	void* loadfunc(const char* func_name);

private:
	DYNLIB_HANDLE handler_;
};

#endif//_MODULEFACTORY_H_

