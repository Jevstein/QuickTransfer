/*
 *  epoll_api.h 
 *  epoll_api
 *
 *  Created by Jevstein on 2018/11/1 17:06.
 *  Copyright @ 2018year Jevstein. All rights reserved.
 *
 *  ���ĺ���:
 *  1.int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
 *    op:     EPOLL_CTL_ADD��EPOLL_CTL_MOD��EPOLL_CTL_DEL
 *    events��EPOLLIN��EPOLLOUT��EPOLLRDHUP��EPOLLPRI��EPOLLERR��EPOLLHUP��EPOLLET��EPOLLONESHOT��EPOLLWAKEUP
 */

#ifndef __EPOLL_API_H__
#define __EPOLL_API_H__

#include "osversion.h"
#include "socket_api.h"

// event
#if defined(YI_OS_MAC) //mac: kqueue
#	define EPOLLIN			(1 << 0)	//#define EVFILT_READ	(-1)    //��ʾ��Ӧ���ļ����������Զ��������Զ�SOCKET�����رգ�
#	define EPOLLOUT			(1 << 1)	//#define EVFILT_WRITE	(-2)    //��ʾ��Ӧ���ļ�����������д
#	define EPOLLPRI			(1 << 2)	//#define EVFILT_AIO	(-3)	/* attached to aio requests */ //��ʾ��Ӧ���ļ��������н�����ݿɶ�������Ӧ�ñ�ʾ�д������ݵ�����
#	define EPOLLERR			(1 << 3)	//#define EVFILT_VNODE	(-4)	/* attached to vnodes */       //��ʾ��Ӧ���ļ���������������
#	define EPOLLHUP			(1 << 4)	//#define EVFILT_PROC	(-5)	/* attached to struct proc */  //��ʾ��Ӧ���ļ����������Ҷ�
#	define EPOLLET			(1 << 5)	//#define EVFILT_SIGNAL	(-6)	/* attached to struct proc */  //��EPOLL��Ϊ��Ե����(Edge Triggered)ģʽ�����������ˮƽ����(Level Triggered)��˵��
#	define EPOLLONESHOT		(1 << 6)	//#define EVFILT_TIMER	(-7)	/* timers */                   //ֻ����һ���¼���������������¼�֮���������Ҫ�����������socket�Ļ�����Ҫ�ٴΰ����socket���뵽EPOLL������
//#	define EPOLLMACHPORT	(1 << 7)	//#define EVFILT_MACHPORT (-8)	/* Mach portsets */
//#	define EPOLLFS			(1 << 8)	//#define EVFILT_FS		(-9)	/* Filesystem events */
//#	define EPOLLUSER		(1 << 9)	//#define EVFILT_USER   (-10)   /* User events */
//										/* (-11) unused */
//#	define EPOLL_EV			(1 << 10)	//#define EV
#else//linux: epoll
#	include <sys/epoll.h>
#	include <sys/wait.h>
#endif//YI_OS_MAC

typedef int YI_EPFD;
typedef void net_io_event;
typedef net_io_event* net_io_event_p;

YI_EPFD io_create(net_io_event** events, int nfds = 4096);
bool io_add_fd(YI_EPFD epfd, YI_SOCKET sockfd, int events, void* key);
bool io_modify_fd(YI_EPFD epfd, YI_SOCKET sockfd, int events, void* key);
bool io_del_fd(YI_EPFD epfd, YI_SOCKET sockfd, int events = 0);
int io_wait(YI_EPFD epfd, net_io_event** events, int maxevents = 2000);
void io_close(YI_EPFD epfd, net_io_event* events);

int io_get_event(net_io_event** events, int idx);
void* io_get_userdata(net_io_event** events, int idx);


#endif //__EPOLL_API_H__