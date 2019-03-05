#ifndef _NET_RECTOR_H_
#define _NET_RECTOR_H_
#include "udp_socket.h"
#include "net_io.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _jvt_net_reactor
{
    jvt_net_io_t net_io;
    // jvt_queue_t que_of_events;
} jvt_net_reactor_t;


int jvt_net_reactor_init(jvt_net_reactor_t *reactor);
void jvt_net_reactor_uninit(jvt_net_reactor_t *reactor);
int jvt_net_reactor_run(jvt_net_reactor_t *reactor);
int jvt_net_reactor_add_event(jvt_net_reactor_t *reactor, udp_socket_t *sock, int events);
int jvt_net_reactor_del_event(jvt_net_reactor_t *reactor, udp_socket_t *sock, int events);
int jvt_net_reactor_mod_event(jvt_net_reactor_t *reactor, udp_socket_t *sock, int events);
// void jvt_net_reactor_push_event(jvt_net_reactor_t *reactor, IEvent* ev) { que_of_event_.push(ev); }sh(ev); }


#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif//_NET_RECTOR_H_