//server.cpp (works with client.cpp)


#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <csignal>
#include <sys/types.h>
#include <sys/socket.h>  // define socket 
#include <netinet/in.h>  // define internet socket 
#include <netdb.h>       // define internet socket 
#include <thread>
#include <vector>
#include <mutex>

#define SERVER_PORT 3935
#define MAXNUMCLIENTS 10

using namespace std;

//*************
// Client Def
//*************
typedef struct Client {
  //Stores the size of the address of the client
  socklen_t clientAddrLen;
  
  //Client Address
  struct sockaddr_in clientAddress;
  
  //Buffer pushed to while reading from client connection
  char buffer[512];
  
  //Number of chars read into buffer
  int numCharRead;
  
  //Client File Descriptor
  int clientFD;
  
  //Client Name
  char name[32];
} Client;
  
//*************
//    Error
//*************  
void error(const char *msg)
{
  perror(msg);
  exit(1);
}

void signalHandler(int signal);

void clientHandler(Client& Client, vector<thread*>& threadList);

//***************
//    Globals
//***************
//Client List
vector<Client> clientList;

//Thread Vector
vector<thread> threads;

int socket_fd;
  
//***************
//     MAIN
//***************
int main()
{
  //VARIABLES
  //Socket & and New Socket File Descriptors
  int newsocket_fd;
  
  //Server Address
  struct sockaddr_in serverAddress;
  
  //Number of chars written
  int numCharWritten;
  
  //Clients
  Client tempClient;
  struct sockaddr_in tempAddress;
  socklen_t tempLen = sizeof(tempAddress);

    
  //Thread Vector
  vector<thread*> threads;
    
  //Mutex for thread locking
  mutex mutex;
  
  //CREATING A STREAM SOCKET
  socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd < 0) //Failed to open
    error("SERVER: Error opening socket"); //Exit program
  cout << "Socket Opened!" << endl;
  
  //Set all values in buffer to zero
  bzero((char *) &serverAddress, sizeof(serverAddress));
  
  //SETTING THE VARIABLES IN SERVERADDRESS
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(SERVER_PORT);
  serverAddress.sin_addr.s_addr = INADDR_ANY;
  
  //Bind the socket for the server to an internet port
  if (bind(socket_fd, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0)
    error("SERVER: Error on binding"); //Exit Program
  cout << "Socket bound!" << endl;
  
  //Listen to the server socket for connecting clients
  if (listen(socket_fd, 4) < 0) //Can have a max of 4 connecting clients while handling other code
    error("SERVER: Error on listen");
  cout << "Listening..." << endl;
    
  // Watch for abort, terminate & interrupt signals 
	signal(SIGABRT, &signalHandler);
	signal(SIGTERM, &signalHandler);
	signal(SIGINT, &signalHandler);
  
  tempClient.clientAddress = tempAddress;
  tempClient.clientAddrLen = tempLen;
  
  //SERVER ACCEPT LOOP: Wait for connection and fork thread upon connection
  while ((newsocket_fd = accept(socket_fd, (struct sockaddr *) &tempAddress, &tempLen)) > 0) 
  {
    if (newsocket_fd < 0)
      error("SERVER: Accept failed"); //Exit Program
    //cout << "Client Accepted!" << endl;
    //Lock Variables
    mutex.lock();
    if (clientList.size() >= MAXNUMCLIENTS)
      cout << "Client attempted to connect, but server is full" << endl;
    else
    {
      tempClient.clientFD = newsocket_fd;
      clientList.push_back(tempClient); //Adding temp client to list
      thread temp([&] {clientHandler(std::ref(tempClient), std::ref(threads));});
      temp.detach();
      threads.push_back(&temp); //New Thread
    }
    mutex.unlock(); //Unlock
  }  
  
  shutdown(socket_fd, SHUT_RDWR);
  mutex.lock();
  mutex.unlock();
  return 0;
}

void clientHandler(Client& Client, vector<thread*>& threadList)
{
  //cout << "Started thread" << endl;
  mutex mutex;
  bzero((char *) &Client.name, sizeof(Client.name));
  bzero((char *) &Client.buffer, sizeof(Client.buffer));
  //strcpy(Client.name, "");
  //memset(Client.buffer, '\0', sizeof(Client.buffer));
  read(Client.clientFD, Client.name, sizeof(Client.name));
  
  strcpy(Client.buffer, "SERVER: ");
  strcat(Client.buffer, Client.name);
  strcat(Client.buffer, " has connected!");
  
  cout << Client.buffer << endl;
  
  mutex.lock();
  for (int i = 0; i < clientList.size(); i++)
  {
    send(clientList[i].clientFD, Client.buffer, strlen(Client.buffer), 0);
  }
  mutex.unlock();
  
  char tempBuffer[512];
  while (read(Client.clientFD, tempBuffer, sizeof(tempBuffer)) > 0)
  {
    memset(Client.buffer, '\0', sizeof(Client.buffer));
    strcpy(Client.buffer, Client.name);
    strcat(Client.buffer, ": ");
    strcat(Client.buffer, tempBuffer);
    
    cout << Client.buffer << endl;
    
    mutex.lock();
    for (int i = 0; i < clientList.size(); i++)
    {
      if (clientList[i].clientFD != Client.clientFD)
      {
        send(clientList[i].clientFD, Client.buffer, strlen(Client.buffer), 0);
      }
    }
    mutex.unlock();
    memset(tempBuffer, '\0', sizeof(tempBuffer));
  }
  
  //Upon disconnect
  memset(Client.buffer, '\0', sizeof(Client.buffer));
  strcpy(Client.buffer, "SERVER: ");
  strcat(Client.buffer, Client.name);
  strcat(Client.buffer, " has disconnected!");
  
  cout << Client.buffer << endl;
  
  mutex.lock();
  for (int i = 0; i < clientList.size(); i++)
  {
    if (clientList[i].clientFD != Client.clientFD)
    {
      send(clientList[i].clientFD, Client.buffer, strlen(Client.buffer), 0);
    }
  }
  
  shutdown(Client.clientFD, SHUT_RDWR);
  
  for (int i = 0; i < clientList.size(); i++)
  {
    if (clientList[i].clientFD == Client.clientFD)
    {
      clientList.erase(clientList.begin() + i);
      threadList.erase(threadList.begin() + i);
      break;
    }
  }
  mutex.unlock();
  return;
}

//SIGNAL HANDLER
void signalHandler(int signal) 
{
  char message[16];
  mutex mutex;
  strcpy(message, "* EXIT *");
  
  mutex.lock();
  for (int i = 0; i < clientList.size(); i++)
  {
    send(clientList[i].clientFD, message, strlen(message), 0);
  }
  mutex.unlock();
  
  cout << "\b\bServer is shutting down..." << endl;
  
  sleep(1);
  
  shutdown(socket_fd, SHUT_RDWR);
  
  cout << "Server has shut down!" << endl;
  
  exit(0);
}