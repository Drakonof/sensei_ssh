#include <list>

#include <sys/types.h>
#include <sys/wait.h>

#include "tcp_server.h"
#include "pstream.h"

void catch_signal(int sig);
void chield_signal_setup(void);
void parent_signal_setup(void);

std::list<int> client_list;



int main(int argc, char **argv)
{
    unsigned int port = 5000;

    net::Tcp_server tcp(port);
    //char b[256];

    

    std::string str;

    int pid_cli = 0;

    parent_signal_setup();

    size_t cli_num = 0;

    LOG("SERVER PARENT PID" __ getpid());

    while (pid_cli = tcp.fork_server()) {

       LOG("cli_num=" << cli_num << ", client_list size=" << client_list.size() << ", new child pid: " << pid_cli);

        if (pid_cli < 0) {
            std::cerr << "server: fork failed"<< std::endl;
             //sleep(1);
            continue;
        }

        cli_num++;
        client_list.push_back(pid_cli);
        LOG("cli_num="<< cli_num << ", client_list.size="<<client_list.size());
    }

close(tcp.sockfd);
tcp.sockfd = 0;

LOG("SERVER CHIELD PID" __ getpid());
chield_signal_setup();

while(1) {

    tcp >> str;
    std::cout << "got string: " << str << std::endl;

    if(str.size() == 0) {
        break;
    }

    redi::ipstream in(str.c_str()); //std::cout << in.rdbuf()

        std::string str_back;
        std::string s;
        while (std::getline(in, s)) 
        {
          str_back += s + '\n';
        }

    tcp << str_back;
   /// tcp << flush();
}
    //std::cout << str_back;
    //tcp << in.rdbuf();

    //close(sock_cli);

   
   // tcp >> b;
    


      exit(0);
}

void catch_child(int sig)
{
    LOG("Получен сигнал: " << sig);

    exit(0);


}

void catch_signal(int sig)
{
    errno = 0;
    LOG("Получен сигнал: " << sig <<", client_listsize()=" << client_list.size());

    if ((sig == SIGTERM) || (sig == SIGINT)) {
        
        for (auto i:client_list)
       // for(std::list<int>::iterator i=client_list.begin();i != client_list.end(); ++i) 
        {
            kill(i, sig);
            LOG("kill: " << i);
        }

        exit(0);
    } 
    
    if (sig == SIGCHLD) {
        int end_status;
        std::list<int>::iterator j = client_list.begin();

        for(std::list<int>::iterator i=client_list.begin();i != client_list.end();)
          {
            if(waitpid(*i,&end_status,WNOHANG)>0)
              {
                LOG("Потомок завершился: " << *i);
                i=client_list.erase(i);
              }
            else
              i++;

            // waitpid(*i, &status,0);

            //     if (WIFSIGNALED(status)){
            //         LOG("Error");
            //     }
            //     else if (WEXITSTATUS(status)){
            //        LOG("Exited Normally");
            //        j = i;

            //     }
          }

          // if (!client_list.empty())
          //       client_list.erase(j);
    }



    // if (sig == SIGCHLD) {
    //     for(std::list<int>::iterator i = client_list.begin(); i != client_list.end(); ++i) {
    //       //  LOG(*i);
    //         if (waitpid(*i, &status, WNOHANG) > 0) {
    //              client_list.erase(i);
    //         }
    //         else
    //             continue;
    //     }


   //    }

    


}


void chield_signal_setup(void)
{
  static sigset_t sigmask;
  static struct sigaction action;
  sigfillset (&sigmask) ;
  sigprocmask(SIG_SETMASK, &sigmask, 0);

  if (
        -1 == sigdelset(&sigmask, SIGTERM) ||    // Ручное прерывание
        -1 == sigdelset(&sigmask, SIGINT) ||    // Если что-то пойдет не так
        -1 == sigdelset(&sigmask, SIGPIPE) ||    // Если хост разорвал связь
        -1 == sigprocmask(SIG_SETMASK, &sigmask, 0)
    ) { 
        exit(errno);
    }

     sigfillset(&action.sa_mask);              // Блокируем все сигналы в обработчике
    action.sa_handler = catch_child;

    if (
        -1 == sigaction(SIGTERM, &action, NULL) ||    // Ручное прерывание
        -1 == sigaction(SIGINT, &action, NULL) ||  
  
        -1 == sigaction(SIGPIPE, &action, NULL)    // Если хост разорвал связь


    ) {
        exit(errno);
    }
    

}


void parent_signal_setup(void)
{
    static sigset_t sigmask;
    static struct sigaction action;

    sigfillset (&sigmask) ;
    sigprocmask(SIG_SETMASK, &sigmask, 0);

    if (
        -1 == sigdelset(&sigmask, SIGTERM) ||    // Ручное прерывание
        -1 == sigdelset(&sigmask, SIGINT) ||    // Если что-то пойдет не так
        -1 == sigdelset(&sigmask, SIGALRM) ||    // Для выхода по таймауту
        -1 == sigdelset(&sigmask, SIGPIPE) ||    // Если хост разорвал связь
        -1 == sigdelset(&sigmask, SIGHUP)  ||    // Чтение конфигурации
        -1 == sigdelset(&sigmask, SIGCHLD)  ||   
        -1 == sigprocmask(SIG_SETMASK, &sigmask, 0)
    ) { 
        exit(errno);
    }

    sigfillset(&action.sa_mask);              // Блокируем все сигналы в обработчике
    action.sa_handler = catch_signal;

    if (
        -1 == sigaction(SIGTERM, &action, NULL) ||    // Ручное прерывание
        -1 == sigaction(SIGINT, &action, NULL) ||  
        -1 == sigaction(SIGALRM, &action, NULL) ||    // Для выхода по таймауту
        -1 == sigaction(SIGPIPE, &action, NULL) ||    // Если хост разорвал связь
        -1 == sigaction(SIGHUP,  &action, NULL) ||
        -1 == sigaction(SIGCHLD,  &action, NULL)
    ) {
        exit(errno);
    }


}
