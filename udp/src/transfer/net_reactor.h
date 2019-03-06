#ifndef _NET_RECTOR_H_
#define _NET_RECTOR_H_
#include "udp_socket.h"
#include "net_io.h"
#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef struct _jvt_net_event
{
    int eid;
    void *data;
    int len;
    // udp_socket_t *sock;
} jvt_net_event_t;

typedef struct _jvt_net_reactor
{
    jvt_net_io_t net_io;
    pthread_t tid;
    int is_started;
    // jvt_queue_t send_events;
    // jvt_queue_t recv_events;
} jvt_net_reactor_t;


int jvt_net_reactor_init(jvt_net_reactor_t *reactor);
void jvt_net_reactor_uninit(jvt_net_reactor_t *reactor);
int jvt_net_reactor_run(jvt_net_reactor_t *reactor);
int jvt_net_reactor_add_event(jvt_net_reactor_t *reactor, udp_socket_t *sock, int events);
int jvt_net_reactor_del_event(jvt_net_reactor_t *reactor, udp_socket_t *sock, int events);
int jvt_net_reactor_mod_event(jvt_net_reactor_t *reactor, udp_socket_t *sock, int events);
int jvt_net_reactor_push_event(jvt_net_reactor_t *reactor, udp_socket_t *sock, jvt_net_event_t* ev);


#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif//_NET_RECTOR_H_