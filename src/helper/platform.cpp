#include "prefixhead.h"
#include "platform.h"

extern int p_thread_create(p_thread_t * thread, p_thread_proc start_routine, p_thread_arg_t arg)
{
#ifdef YI_OS_WIN32
	unsigned int dwThreadId;

	*thread = (HANDLE)_beginthreadex(NULL, 0, start_routine, arg, 0, &dwThreadId);
	if (!*thread) return -1; 
	else return 0;
#else
	return pthread_create(thread, NULL, start_routine, arg);
#endif
}

extern int p_thread_wait_exit(p_thread_t * thread, int timeout)
{
#ifdef YI_OS_WIN32
	if( NULL != *thread )
	{
		WaitForSingleObject(*thread, timeout);
		CloseHandle(*thread);
		*thread = NULL;
	}
	return 0;
#else
	pthread_join(*thread, 0);
	return 0;
#endif
}

extern const char* p_last_error()
{
#ifdef YI_OS_WIN32
	LPTSTR lpMsg = NULL;
	DWORD dwLastError = GetLastError();
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwLastError, GetUserDefaultLangID(), (LPTSTR)&lpMsg, 0, NULL);
	return lpMsg;
#else
	return dlerror();
#endif
}

extern const char* p_socket_last_error(int *error/* = NULL*/)
{
#ifdef YI_OS_WIN32
	static char err_buf[512] = { 0 };
	memset(err_buf, 0, sizeof(err_buf));
	DWORD dwError = WSAGetLastError();
	switch (dwError)
	{
	case WSANOTINITIALISED: p_snprintf(err_buf, sizeof(err_buf), "%d: %s", dwError, "A successful WSAStartup call must occur before using this function."); break;
	case WSAENETDOWN:		p_snprintf(err_buf, sizeof(err_buf), "%d: %s", dwError, "The network subsystem has failed."); break;
	case WSAEACCES:			p_snprintf(err_buf, sizeof(err_buf), "%d: %s", dwError, "The requested address is a broadcast address, but the appropriate flag was not set. Call setsockopt with the SO_BROADCAST parameter to allow the use of the broadcast address."); break;
	case WSAEINVAL:			p_snprintf(err_buf, sizeof(err_buf), "%d: %s", dwError, "An unknown flag was specified, or MSG_OOB was specified for a socket with SO_OOBINLINE enabled."); break;
	case WSAEINTR:			p_snprintf(err_buf, sizeof(err_buf), "%d: %s", dwError, "A blocking Windows Sockets 1.1 call was canceled through WSACancelBlockingCall."); break;
	case WSAEINPROGRESS:	p_snprintf(err_buf, sizeof(err_buf), "%d: %s", dwError, "A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function."); break;
	case WSAEFAULT:			p_snprintf(err_buf, sizeof(err_buf), "%d: %s", dwError, "The buf or to parameters are not part of the user address space, or the tolen parameter is too small."); break;
	case WSAENETRESET:		p_snprintf(err_buf, sizeof(err_buf), "%d: %s", dwError, "The connection has been broken due to keep-alive activity detecting a failure while the operation was in progress."); break;
	case WSAENOBUFS:		p_snprintf(err_buf, sizeof(err_buf), "%d: %s", dwError, "No buffer space is available."); break;
	case WSAENOTCONN:		p_snprintf(err_buf, sizeof(err_buf), "%d: %s", dwError, "The socket is not connected (connection-oriented sockets only)."); break;
	case WSAENOTSOCK:		p_snprintf(err_buf, sizeof(err_buf), "%d: %s", dwError, "The descriptor is not a socket."); break;
	case WSAEOPNOTSUPP:		p_snprintf(err_buf, sizeof(err_buf), "%d: %s", dwError, "MSG_OOB was specified, but the socket is not stream-style such as type SOCK_STREAM, OOB data is not supported in the communication domain associated with this socket, or the socket is unidirectional and supports only receive operations."); break;
	case WSAESHUTDOWN:		p_snprintf(err_buf, sizeof(err_buf), "%d: %s", dwError, "The socket has been shut down; it is not possible to sendto on a socket after shutdown has been invoked with how set to SD_SEND or SD_BOTH."); break;
	case WSAEWOULDBLOCK:	p_snprintf(err_buf, sizeof(err_buf), "%d: %s", dwError, "The socket is marked as nonblocking and the requested operation would block."); break;
	case WSAEMSGSIZE:		p_snprintf(err_buf, sizeof(err_buf), "%d: %s", dwError, "The socket is message oriented, and the message is larger than the maximum supported by the underlying transport."); break;
	case WSAEHOSTUNREACH:	p_snprintf(err_buf, sizeof(err_buf), "%d: %s", dwError, "The remote host cannot be reached from this host at this time."); break;
	case WSAECONNABORTED:	p_snprintf(err_buf, sizeof(err_buf), "%d: %s", dwError, "The virtual circuit was terminated due to a time-out or other failure. The application should close the socket as it is no longer usable."); break;
	case WSAECONNRESET:		p_snprintf(err_buf, sizeof(err_buf), "%d: %s", dwError, "The virtual circuit was reset by the remote side executing a hard or abortive close. For UPD sockets, the remote host was unable to deliver a previously sent UDP datagram and responded with a \"Port Unreachable\" ICMP packet. The application should close the socket as it is no longer usable."); break;
	case WSAEADDRNOTAVAIL:	p_snprintf(err_buf, sizeof(err_buf), "%d: %s", dwError, "The remote address is not a valid address, for example, ADDR_ANY."); break;
	case WSAEAFNOSUPPORT:	p_snprintf(err_buf, sizeof(err_buf), "%d: %s", dwError, "Addresses in the specified family cannot be used with this socket."); break;
	case WSAEDESTADDRREQ:	p_snprintf(err_buf, sizeof(err_buf), "%d: %s", dwError, "A destination address is required."); break;
	case WSAENETUNREACH:	p_snprintf(err_buf, sizeof(err_buf), "%d: %s", dwError, "The network cannot be reached from this host at this time."); break;
	case WSAETIMEDOUT:		p_snprintf(err_buf, sizeof(err_buf), "%d: %s", dwError, "The connection has been dropped, because of a network failure or because the system on the other end went down without notice."); break;
	default:				p_snprintf(err_buf, sizeof(err_buf), "%d: %s", dwError, "Unknown socket error."); break;
	}
	if (error)
		*error = (int)dwError;
	return err_buf;
#else
	if (error)
		*error = errno;
	return strerror(errno);
#endif
}

extern int p_gettimeofday(struct timeval *tp, void *tzp)
{
#ifdef YI_OS_WIN32
	time_t clock;
	struct tm tm;
	SYSTEMTIME wtm;
	GetLocalTime(&wtm);
	tm.tm_year = wtm.wYear - 1900;
	tm.tm_mon = wtm.wMonth - 1;
	tm.tm_mday = wtm.wDay;
	tm.tm_hour = wtm.wHour;
	tm.tm_min = wtm.wMinute;
	tm.tm_sec = wtm.wSecond;
	tm.tm_isdst = -1;
	clock = mktime(&tm);
	tp->tv_sec = (long)clock;
	tp->tv_usec = wtm.wMilliseconds * 1000;
#else
	gettimeofday(tp, (struct timezone *)tzp);
#endif
	return 0;
}

extern void p_sleep(int milli_seconds)
{
#ifdef YI_OS_WIN32
	Sleep((DWORD)milli_seconds);
#else
	int micro_seconds = milli_seconds * 1000;
	usleep(micro_seconds);
#endif
}

extern bool p_atomic_cas(volatile long *obj, long expected, long desired)
{
	//原子操作：c++11跨平台--atomic_compare_exchange_weak

#ifdef YI_OS_WIN32
	long old_obj = *obj;
	return (old_obj == InterlockedCompareExchange(obj, desired, expected)) ? true : false;
#else
	return __sync_bool_compare_and_swap (obj, expected, desired);
#endif
}
