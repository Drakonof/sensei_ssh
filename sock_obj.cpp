#include <string.h>

#include "sock_obj.h"

namespace net
{

Sock_obj::Sock_obj(unsigned int port) :
    sockfd (0)
{
    memset(this,0,sizeof(sockaddr_in));

    sin_port = htons(port);
    sin_family = AF_INET;
    sin_addr.s_addr = htonl(INADDR_ANY);


    if ((sockfd = socket(sin_family, SOCK_STREAM, 0)) < 0)
    {
        throw "Sock_obj: socket failed\n";
    }


}

Sock_obj::~Sock_obj()
{
    close(sockfd);
}

}
