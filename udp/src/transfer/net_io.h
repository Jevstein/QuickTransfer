/*
 *  net_io.h 
 *  net_io
 *
 *  Created by Jevstein on 2018/6/19 10:20.
 *  Copyright @ 2018year Jevstein. All rights reserved.
 *
 */
#ifndef _NET_IO_H_
#define _NET_IO_H_
#include "os_version.h"

// event
#if defined(JVT_OS_MAC) //mac: kqueue
#	define EPOLLIN			(1 << 0)	//#define EVFILT_READ	(-1)    //表示对应的文件描述符可以读（包括对端SOCKET正常关闭）
#	define EPOLLOUT			(1 << 1)	//#define EVFILT_WRITE	(-2)    //表示对应的文件描述符可以写
#	define EPOLLPRI			(1 << 2)	//#define EVFILT_AIO	(-3)	/* attached to aio requests */ //表示对应的文件描述符有紧急的数据可读（这里应该表示有带外数据到来）
#	define EPOLLERR			(1 << 3)	//#define EVFILT_VNODE	(-4)	/* attached to vnodes */       //表示对应的文件描述符发生错误
#	define EPOLLHUP			(1 << 4)	//#define EVFILT_PROC	(-5)	/* attached to struct proc */  //表示对应的文件描述符被挂断
#	define EPOLLET			(1 << 5)	//#define EVFILT_SIGNAL	(-6)	/* attached to struct proc */  //将EPOLL设为边缘触发(Edge Triggered)模式，这是相对于水平触发(Level Triggered)来说的
#	define EPOLLONESHOT		(1 << 6)	//#define EVFILT_TIMER	(-7)	/* timers */                   //只监听一次事件，当监听完这次事件之后，如果还需要继续监听这个socket的话，需要再次把这个socket加入到EPOLL队列里
//#	define EPOLLMACHPORT	(1 << 7)	//#define EVFILT_MACHPORT (-8)	/* Mach portsets */
//#	define EPOLLFS			(1 << 8)	//#define EVFILT_FS		(-9)	/* Filesystem events */
//#	define EPOLLUSER		(1 << 9)	//#define EVFILT_USER   (-10)   /* User events */
//										/* (-11) unused */
//#	define EPOLL_EV			(1 << 10)	//#define EV
#	include <sys/event.h>
#else//linux: epoll
#	include <sys/types.h>
#	include <sys/socket.h>
#	include <sys/epoll.h>
#	include <sys/wait.h>
#endif//YI_OS_MAC

#ifdef __cplusplus
extern "C" {
#endif

enum IO_MODEL
{
	IO_ANY = 0,
	IO_EPOLL,
	IO_KQUEUE,
	IO_SELECT,
};

typedef struct _jvt_net_io
{
	int fd;
	int model;
	void *events;
	int max_events;
} jvt_net_io_t;

int jvt_netio_init(jvt_net_io_t *net_io, int fds/* = 4096*/, int model/* = -1*/);
void jvt_netio_uninit(jvt_net_io_t *net_io);
int jvt_netio_add_fd(jvt_net_io_t *net_io, int sockfd, int events, void* key);
int jvt_netio_del_fd(jvt_net_io_t *net_io, int sockfd, int events);
int jvt_netio_modify_fd(jvt_net_io_t *net_io, int sockfd, int events, void* key);
int jvt_netio_wait(jvt_net_io_t *net_io);
void jvt_netio_close(jvt_net_io_t *net_io);
int jvt_netio_get_event(jvt_net_io_t *net_io, int idx);
void* jvt_netio_get_key(jvt_net_io_t *net_io, int idx);

#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif//_NET_IO_H_
