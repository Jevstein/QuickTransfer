/*
 *  IOModel.h 
 *  IOModel
 *
 *  Created by Jevstein on 2018/6/19 10:20.
 *  Copyright @ 2018year Jevstein. All rights reserved.
 *
 */
#ifndef _IO_MODEL_H_
#define _IO_MODEL_H_

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
#	include <sys/types.h>
#	include <sys/socket.h>
#	include <sys/epoll.h>
#	include <sys/wait.h>
#endif//YI_OS_MAC

class CIOModel
{
public:
	enum {
		IO_ANY = 0,
		IO_EPOLL,
		IO_KQUEUE,
		IO_SELECT,
	};

	static CIOModel *create_model(int type = 0);
	static void destry_model(CIOModel *mode);

public:
	CIOModel(int type);
	virtual ~CIOModel();
	virtual int init(int nfds = 4096) = 0;
	virtual int add_fd(int sockfd, int events, void* key)  = 0;
	virtual int del_fd(int sockfd, int events = 0)  = 0;
	virtual int modify_fd(int sockfd, int events, void* key)  = 0;
	virtual int wait() = 0;
	virtual void close() = 0;

	virtual int get_event(int idx) = 0;
	virtual void* get_key(int idx) = 0;

	int get_mode_type() const { return model_type_; }

protected:
	int fd_;	//epoll|kqueue fd
	int model_type_;
};

#endif//_IO_MODEL_H_
