#include <signal.h>
#include <string.h>

#include "tcp_server.h"
#include "log.h"

namespace net
{    
    

    Tcp_server::Tcp_server(unsigned int port) : 
        Sock_obj(port), sock_cli(0), pid_cli(0), sel(0) // timeout.tv_sec(20)
    {
        timeout.tv_sec  = 0;
        timeout.tv_usec = 0;

        FD_ZERO(&set);
        FD_SET(sockfd, &set);

        int status;
        if ((status = bind(sockfd, *this, sizeof(sockaddr_in))) < 0) { //?
            ERR(strerror(errno));
        }

        if (listen(sockfd, 5) < 0) {
            ERR(strerror(errno));
        }
    }

    Tcp_server::~Tcp_server(void)
    {

    }

    int Tcp_server::fork_server(long int timeout_sec)
    {
        

        // while(true)
        // {
            timeout.tv_sec = 0;//timeout_sec;
           
       

            do {
                sel = select(sockfd + 1, &set, NULL, NULL, 0);//&timeout); //? sockfd + 1

                if (0 == sel) {
                    std::cerr << "server: timeout occurred " << timeout.tv_sec << " (second)" << std::endl;
                    exit(errno);
                }

             } while(sel == -1);

            sockaddr_in cli_adr = {};
            socklen_t addrlen; 

            sock_cli = accept(sockfd, (struct sockaddr *)&cli_adr, &addrlen);
            LOG1(sock_cli);
      
             if(-1 == sock_cli)
             {
                 sleep(1);
                 exit(errno);
             }

             pid_cli = fork();
             LOG("FORK" __ pid_cli);

             // if (pid_cli < 0) {
             //    std::cerr << "server: fork failed"<< std::endl;
             //    return pid_cli;
             // }

             // if (0 != pid_cli) {
             //     close(sock_cli);
             //     return pid_cli;
             // }
             // else {

                 

             //     std::cout << "In a filial: " << getpid() << std::endl;
             //     close(sockfd);

                 
                
             // }
      //  }

             return pid_cli;
    }

    void Tcp_server::operator <<(std::string &str) //how use string?
    {
        //char buf[str.size()];
        time_t ticks = time(NULL);
        //snprintf(str.c_str(), str.size(), "%.24s\r\n", ctime(&ticks));  
        
       // str += ctime(&ticks);
        
        std::cout << write(sock_cli, str.c_str(), str.size()) << "  " << str.size() << std::endl;
        sync();
       // close(sock_cli);
    }

}