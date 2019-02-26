/*
 *  platform.h 
 *  platform
 *
 *  Created by Jevstein on 2018/3/28 19:24.
 *  Copyright @ 2018year Jevstein. All rights reserved.
 *
 */
#ifndef _PLATFORM_H
#define _PLATFORM_H
#include "osversion.h"

/************************************************************************/
/* head file & declaration(macro)                                       */
/************************************************************************/
typedef void * p_thread_arg_t;
#ifdef YI_OS_WIN32
	#include <WinSock2.h>
	#include <windows.h>
	#include <process.h>
	#include <time.h>
	#include <stdint.h>
	//#include "yistdint.h"
	#define P_THREAD_PROC		WINAPI
	#define p_thread_detach(thread)	(0)
	#define p_thread_exit(retval)	ExitThread(retval)
	#define p_thread_self()		GetCurrentThread()
	#define p_thread_cancel(thread)	TerminateThread(thread, -1)
	//#define p_thread_wait(thread) WaitForSingleObject(thread, INFINITE)
	typedef HANDLE			p_thread_t;
	typedef unsigned int	p_thread_retval_t;
	typedef p_thread_retval_t  (P_THREAD_PROC * p_thread_proc)(p_thread_arg_t);

	typedef CRITICAL_SECTION	p_mutex_t;
	#define p_mutex_init(mutex)	InitializeCriticalSection(mutex)
	#define p_mutex_lock(mutex)	EnterCriticalSection(mutex)
	#define p_mutex_trylock(mutex)	TryEnterCriticalSection(mutex)
	#define p_mutex_unlock(mutex)	LeaveCriticalSection(mutex)
	#define p_mutex_destroy(mutex)	DeleteCriticalSection(mutex)

	#define P_EXPORT_DLL		__declspec(dllexport)
	#define DYNLIB_HANDLE HINSTANCE
	#define DYNLIB_LOAD( a ) LoadLibrary( a )
	#define DYNLIB_GETSYM( a, b ) GetProcAddress( a, b )
	#define DYNLIB_UNLOAD( a ) (bool)!FreeLibrary( a )

	#define p_vscprintf _vscprintf
	#define p_vsnprintf _vsnprintf
	//#define p_snprintf _snprintf
	#define p_snprintf _snprintf_s

	typedef SOCKET YI_SOCKET;
	#define CLOSE_SOCKET(fd) closesocket(fd)
	
	#define __YI_FUNC__ __FUNCTION__
	#define __FILENAME__ (strrchr(__FILE__, '/') ? (strrchr(__FILE__, '/') + 1) : __FILE__)
	
#else //!YI_OS_WIN32
	#include <dlfcn.h>

	//#ifdef HAVE_PTHREAD_H
	#include <pthread.h>
	#include <unistd.h>
	#include <sys/select.h>
	#include <sys/time.h>
	//#endif

	#define P_THREAD_PROC
	#define p_thread_detach(thread)	 pthread_detach(thread)
	#define p_thread_exit(retval)	pthreadn_exit(retval)
	#define p_thread_self()		pthread_self()
	#define p_thread_cancel(thread)	pthread_cancel(thread)
	//#define p_thread_wait(thread, ) pthread_join(thread, INFINITE)

	typedef pthread_t		p_thread_t;
	typedef void *			p_thread_retval_t;
	typedef p_thread_retval_t  (P_THREAD_PROC * p_thread_proc)(p_thread_arg_t);

	#ifndef HAVE_PTHREAD_MUTEX_RECURSIVE_NP
	# ifdef PTHREAD_MUTEX_RECURSIVE
	#   define PTHREAD_MUTEX_RECURSIVE_NP PTHREAD_MUTEX_RECURSIVE
	# else
	#   define PTHREAD_MUTEX_RECURSIVE_NP 0
	# endif
	#endif

	#ifdef HAVE_PTHREAD_MUTEXATTR_SETKIND_NP
	/* ft, no defination found in pthread.h! */
	extern int pthread_mutexattr_setkind_np(pthread_mutexattr_t *__attr, int __kind);
	#  define p_mutexattr_settype(attr, kind)	pthread_mutexattr_setkind_np(attr, kind)
	#else
	#  ifdef HAVE_PTHREAD_MUTEXATTR_SETTYPE
	#    define p_mutexattr_settype(attr, kind)	pthread_mutexattr_settype(attr, kind)
	#  else
	#    define p_mutexattr_settype(attr, kind) //(0)
	#  endif
	#endif

	/*
	 * ??? shall I usr pthread_mutexattr_setkind_np ???
	 * pthread_mutexattr_setkind_np(&_mutex_attr, PTHREAD_MUTEX_RECURSIVE_NP);
	 */

	typedef pthread_mutex_t		p_mutex_t;
	#define p_mutex_init(mutex)\
		do {	\
			pthread_mutexattr_t     _mutex_attr;	\
			pthread_mutexattr_init(&_mutex_attr);	\
			p_mutexattr_settype(&_mutex_attr, PTHREAD_MUTEX_RECURSIVE_NP);	\
			pthread_mutex_init(mutex, &_mutex_attr);	\
			pthread_mutexattr_destroy(&_mutex_attr);	\
		} while(0);

	#define p_mutex_lock(mutex)	pthread_mutex_lock(mutex)
	#define p_mutex_trylock(mutex)	pthread_mutex_trylock(mutex)
	#define p_mutex_unlock(mutex)	\
		do {	\
			pthread_mutex_unlock(mutex);	\
		}while(0);
	#define p_mutex_destroy(mutex)	pthread_mutex_destroy(mutex)

	#define P_EXPORT_DLL
	#define DYNLIB_HANDLE void*
	#define DYNLIB_LOAD( a ) dlopen( a, RTLD_NOW )
	#define DYNLIB_GETSYM( a, b ) dlsym( a, b )
	#define DYNLIB_UNLOAD( a ) dlclose( a )

	//#define p_vscprintf vscprintf//linux下无此API
	//#define p_vscprintf(fmt, args) vsnprintf(0, 0, fmt, args)//未拷贝导致后续vsnprintf失败
	#define p_vscprintf(fmt, args)({\
		int ret = 0;\
		va_list arg_copy;\
		va_copy(arg_copy, args);\
		ret = vsnprintf(NULL, 0, fmt, arg_copy);\
		va_end(arg_copy);\
		(ret);\
	})
	#define p_vsnprintf vsnprintf
	#define p_snprintf snprintf

	typedef int YI_SOCKET;
	#define CLOSE_SOCKET(fd) ::close(fd)

	#define __YI_FUNC__ __func__
	#define __FILENAME__ __FILE__

	//#ifndef max
	//#	define max(a,b)		(((a) > (b)) ? (a) : (b))
	//#endif

	//#ifndef min
	//#	define min(a,b)		(((a) < (b)) ? (a) : (b))
	//#endif

#endif //YI_OS_WIN32

/************************************************************************/
/* system functions                                                     */
/************************************************************************/
extern int p_thread_create(p_thread_t * thread, p_thread_proc start_routine, p_thread_arg_t arg);
extern int p_thread_wait_exit(p_thread_t * thread, int timeout = 0xFFFFFFFF);
extern void p_sleep(int milli_seconds);
extern const char* p_last_error();
extern const char* p_socket_last_error(int *error = NULL);
extern int p_gettimeofday(struct timeval *tp, void *tzp);
extern bool p_atomic_cas(volatile long *obj, long expected, long desired);

/************************************************************************/
/* parameter type define                                                */
/************************************************************************/
#ifndef IN
#	define IN
#endif // !IN
#ifndef OUT
#	define OUT
#endif // !OUT
#ifndef INOUT
#	define INOUT
#endif // !INOUT

#endif //_PLATFORM_H