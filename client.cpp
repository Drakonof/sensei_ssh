#include <fstream>
#include <getopt.h>
#include <unistd.h>

#include <ext/rope>


#include <readline/readline.h>
#include <readline/history.h>

#include "tcp_client.h"


static std::string read_command(std::string host_name) 
{
    char* buf = nullptr;

    std::string res;  //empty?
   // std::rope res;
  
    if (nullptr != (buf = readline(host_name.c_str()))) {
        if (strlen(buf) > 0) {
            add_history(buf);
        }

        res = buf;

        // readline malloc's a new buffer every time.
        free(buf);
    }

    return res;
}


int main(int argc, char **argv)
{
    int opt = 0;

    unsigned int port = 5000;

    std::string address = "127.0.0.1";
    std::string command = "hostname";
    std::string command_back_data;
    std::string host_name;

    option long_options[] =
      {
        {"port",     1,0, 'p'},
     // {"timeout",  1,0, 't'},
        {"help",     0,0, 'h'},
        {"cmd",      1,0, 'c'},
        {"addr",      1,0, 'a'},
        {0, 0, 0, 0}
      };

    while(true) {
        if ((opt = getopt_long (argc, argv, "hp:c:a:",long_options, NULL)) == EOF) {
            break;
        }

        switch (opt) {
        case '?':
            std::cerr << "Unknown option: " << (char)optopt<< std::endl;
            //todo: usage();
            exit(EXIT_FAILURE);
        break;
        case 'h':
            //todo: usage();
            exit(EXIT_SUCCESS);
        break;
        case 'c':
            command = optarg;
        break;
        case 'a':
            address = optarg;
        break;
        case 'p':
            port = atoi(optarg);
        break;
        default:
            //todo: usage();
        break;
        }
    }

    net::Tcp_client tcp(address.c_str(), port);
    
    tcp << command;
    tcp >> host_name;

    LOG1(getpid());

    host_name.pop_back();
    host_name ='[' +std::to_string(getpid())+ "] " +  host_name + std::string(": ");

    //todo: exit from while loop
    do {
        command = read_command(host_name);

        if (command.empty()) {
            continue;
        }

        if ('q' == command.back()) {
            break;
        }

        tcp << command;
        tcp >> command_back_data;
        sync();

        std::cout << command_back_data << std::endl;
        
    } while (1);

	exit(EXIT_SUCCESS);
}
