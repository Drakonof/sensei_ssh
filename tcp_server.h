#if defined(TCP_SERVER_H) == 0
#define TCP_SERVER_H

#include <string.h>

#include "sock_obj.h"

namespace net
{

class Tcp_server : public Sock_obj
{
private:
    int sock_cli;
    pid_t pid_cli;
    timeval timeout;
    fd_set set;
    int sel;

    void deamon_signal_setup_(void);
    
      

public:
    Tcp_server(unsigned int port);
     ~Tcp_server(void);

    // void catch_signal_(int sig);
    // void chield_signal_setup_(void);

    int fork_server(long int timeout_sec = 20);
    
    void operator <<(std::string &str);
    //std::ostream &operator <<(const char* buf);

    void operator >> (std::string &str)
    {
     char buf[BUFSIZ] = {};

    // read(sockfd, buf, BUFSIZ);

     int len=recv(sock_cli, buf, BUFSIZ, 0);
     LOG("recv " << buf __ strerror(errno));
     LOG("recvlen= " << len);


     str = buf;
    
    }





    operator const sockaddr*() const {LOG("to : ");return (const sockaddr*)this;}
};
}

#endif
