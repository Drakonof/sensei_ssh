#if defined(TCP_CLIENT_H) == 0
#define TCP_CLIENT_H

#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <ext/rope>

#include "sock_obj.h"

namespace net
{

class Tcp_client : public Sock_obj {
private:
    
public:
    Tcp_client(const char *host, unsigned int port);
    virtual ~Tcp_client();
    
    void operator >>(char *buf);
    void operator >>(std::string &str);
    void operator <<(std::string &str);
};

}

#endif