#ifndef BRKS_INTF_INTERFACE_H_
#define BRKS_INTF_INTERFACE_H_

// #include "protocol_head.h"
// #include "protocol_codec.h"
// #include "json_protocol_codec.h"

#include "../common/event.h"
#include "../common/protocol_codec.h"

class CReactor
{
public:
    CReactor(){}
    // CReactor(std::function< iEvent* (const IEvent*)>  callback) : callback_(callback)
    // {
    //     codecs_[JSON_PROTOCOL_TYPE]   = new json_protocol_codec_t;
    //     codecs_[PB_PROTOCOL_TYPE]     = NULL;
    //     codecs_[FB_PROTOCOL_TYPE]     = NULL;
    //     codecs_[BINARY_PROTOCOL_TYPE] = NULL;
    // };

    // bool add_server_socket(int socket);
    // bool close();
    // void run();

    // int create_and_bind_socket(unsigned short port);

    

private:
    std::function<IEvent* (const IEvent*)> callback_;
    // int server_socket_;
    // int epoll_fd_;

    protocol_codec_t* codecs_[4]; // there is only 4 protocol codec.

private:
    // bool add_epoll_event(int efd, int socket, int events);
    // bool accept_client(int efd, int sfd);
    // unsigned int read_client_data(int fd, char* buffer, int size);
    // int nio_recv(int sockfd, char *buffer, int length, int *ret);
    // int nio_write(int fd, char* buf, int len);
};

#endif