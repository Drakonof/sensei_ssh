#include <iostream>
#include <cstring>

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/socket.h>



#include "tcp_client.h"


namespace net
{    


Tcp_client::Tcp_client(const char *host, unsigned int port) : Sock_obj(port)
{
    int opt = 1;

    sin_addr.s_addr = inet_addr(host);
    sockaddr_in cl = *this;

    if (sockfd < 0) {
        throw ("sockbuf::CreateClient: ssl_connect_to(): socket() failed" );
    }

      
    if (-1 == (setsockopt(sockfd,SOL_SOCKET, SO_REUSEADDR, (void*)&opt, sizeof(int)))) {    
        throw ("sockbuf::CreateClient: error setsockopt");
    }

    if (connect(sockfd, (const sockaddr*)&cl, sizeof(sockaddr_in)) < 0) {
           ERR(strerror(errno));
    }
}


Tcp_client::~Tcp_client()
{
   shutdown(sockfd, SHUT_RDWR);
   close(sockfd);
}

void Tcp_client::operator >>(char *buf)
{
    memset(buf, 0, BUFSIZ);

    int byte_num = read(sockfd, buf, BUFSIZ);

    if (byte_num < 0) {
        //todo: throw
    }
}

void Tcp_client::operator >>(std::string &str)
{
    char buf[BUFSIZ];
    memset(buf, 0, BUFSIZ);

    int byte_num = 0;

    str.clear();

    do {
        byte_num = read(sockfd, buf, BUFSIZ);

        if (byte_num < 0) {
            //todo: throw
        }

        str += buf;
    } while (byte_num == BUFSIZ);
    
    
}

void Tcp_client::operator <<(std::string &str)
{
    send(sockfd, str.c_str(), str.size(), 0);
}


}
