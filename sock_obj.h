#if defined(SOCK_OBJ_H) == 0
#define SOCK_OBJ_H

#include <iostream>

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "log.h"

namespace net
{

class Sock_obj : public sockaddr_in
{
public:
    int sockfd;

public:

    Sock_obj(unsigned int port);
     ~Sock_obj();
};

}

#endif