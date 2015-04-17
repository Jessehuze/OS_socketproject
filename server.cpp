//server.cpp (works with client.cpp)


#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>  /* define socket */
#include <netinet/in.h>  /* define internet socket */
#include <netdb.h>       /* define internet socket */
#include <thread>

#define SERVER_PORT 3932

using namespace std;

int main()
{
  int socket_fd, newsocket_fd, portNum;
  socklen_t clientAddrLen;
  char buffer[256];
  struct sockaddr_in serverAddress;
  vector<struct sockaddr_in> ClientAddresses;
  
  
  return 0;
}