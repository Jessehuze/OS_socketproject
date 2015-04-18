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

#DEFINE SERVER_PORT 3932
#DEFINE MAXNUMCLIENTS 10

using namespace std;


//*************
// Client Def
//*************
typedef struct Client {
  //Stores the size of the address of the client
  socklen_t clientAddrlen;
  
  //Client Address
  struct sockaddr_in clientAddress;
  
  //Buffer pushed to while reading from client connection
  char buffer[512];
  
  //Number of chars read into buffer
  int numCharRead;
  
  //Client File Descriptor
  int clientFD;
  
  //Client Name
} Client;
  
//*************
//    Error
//*************  
void error(const char *msg)
{
  perror(msg);
  exit(1);
}
  
//***************
//     MAIN
//***************
int main()
{
  //VARIABLES
  //Socket & and New Socket File Descriptors
  int socket_fd, newsocket_fd;
  
  //Server Address
  struct sockaddr_in serverAddress;
  
  //Number of chars written
  int numCharWritten;
  
  //Clients
  Client tempClient;
  tempClient.clientAddress = {AF_INET};
  tempClient.clientAddrLen = sizeof(_Clients[i].clientAddress);
  vector<Client> Clients;
  
  /*for (int i = 0; i < MAXNUMCLIENTS; i++) {
    _Clients[i].clientAddress = {AF_INET};
    _Clients[i].clientAddrLen = sizeof(_Clients[i].clientAddress);*/
    
  //Thread Vector
  vector<thread> threads;
    
  //Mutex for thread locking
  mutex mutex;
  
  //CREATING A STREAM SOCKET
  socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd < 0) //Failed to open
    error("SERVER: Error opening socket"); //Exit program
  
  //Set all values in buffer to zero
  bzero((char *) &serverAddress, sizeof(serverAddress));
  
  //SETTING THE VARIABLES IN SERVERADDRESS
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(SERVER_PORT);
  serverAddress.sin_adr.s_addr = INADDR_ANY;
  
  //Bind the socket for the server to an internet port
  if (bind(socket_fd, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0)
    error("SERVER: Error on binding"); //Exit Program
  
  //Listen to the server socket for connecting clients
  if (listen(socket_fd, 5) < 0) //Can have a max of 5 connecting clients while handling other code
    error("SERVER: Error on listen");
  
  //SERVER ACCEPT LOOP: Wait for connection and fork thread upon connection
  while ((newsocket_fd = accept(socket_fd, (struct sockaddr *) &tempClient.clientAddress, &tempClient.clientAddrLen))
    if (newsocket_fd < 0)
      error("SERVER: Accept failed"); //Exit Program
    //Lock Variables
    mutex.lock();
    if (Clients.size >= MAXNUMCLIENTS)
      cout << "Client attempted to connect, but server is full" << endl;
    else
    {
      tempClient.clientFD = newsocket_fd;
      Clients.push_back(tempClient); //Adding temp client to list
      threads.push_back(thread(clientHandler(tempClient))); //New Thread
    }
    mutex.unlock(); //Unlock
  }
  
  //Interrupt Handling
  
  return 0;
}

void clientHandler(Client Client)
{
  
}