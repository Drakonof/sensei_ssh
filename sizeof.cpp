
#include "log.h"

using namespace std;
//int CreateServer(const unsigned port,int& sock,sockaddr_in& sin);
void  Reset(int& sock);

class A
{
   // int i:3;
   // unsigned u:5;

   // int d:31;
   // char ch;*/
   // char sh[17] __attribute__((__aligned__(8)));

  //  A* pa;
    std::string str;
    static std::iostream iostr;
   // virtual  int virtFunc() ;
};

int main(int argc, char **argv)
{
    try
     {
        LOG1(sizeof(A));
     }
    catch (const char* err)
     {
        ERR(err);
     }

     const int c_int = 100;

    return 0;
}

