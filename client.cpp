/************************************************************************/ 
/*   PROGRAM NAME: client.c  (works with serverX.c)                     */ 
/*                                                                      */ 
/*   Client creates a socket to connect to Server.                      */ 
/*   When the communication established, Client writes data to server   */ 
/*   and echoes the response from Server.                               */ 
/*                                                                      */ 
/*   To run this program, first compile the server_ex.c and run it      */ 
/*   on a server machine. Then run the client program on another        */ 
/*   machine.                                                           */ 
/*                                                                      */ 
/*   COMPILE:    gcc -o client client.c -lnsl                           */ 
/*   TO RUN:     client  server-machine-name                            */ 
/*                                                                      */ 
/************************************************************************/ 
#include <iostream> 
#include <stdio.h> 
#include <cstdlib>
#include <csignal>
#include <sys/types.h> 
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include unistd.h>
#include <sys/socket.h>  // define socket
#include <netinet/in.h>  // define internet socket
#include <netdb.h>       // define internet socket
#include <thread>
using namespace std;
 
#define SERVER_PORT 3932     // define a server port number 
 
 //for the handling of ctrl+c
 void SignalHandler(int sig)
 {
	if(SIGINT)
	{
		cout << "Please type /exit , /quit , or /part to leave the chat room" << endl;
	}
	return;
 }
 /*
 void* readServer(void* dmyptr)
 {
	char buf[256];
	memset(buf, '\0', 256);
	bool close = false;
	
	while(read(socketFileDescriptor, buf, 255))
	{
		if(strlen(buf) == 11 && strcmp(strstr(buffer, " *"), " *") == 0)
		{
			
		}
	}
	return;
 }
*/
int main() 
{ 
    int socketFileDescriptor; 
    char buf[512]; 
	char clientNickname[64];
	char clientRemove[64]
	char hostName[64];
	struct hostent* hp; 
	thread fromServerThread; // waits for input from server and prints it to the screen
	SignalHandler(SIGINT, &interuptHandler);//for interupt handling
	
	
	//Get the server and client nickname
	cout << "Please enter a server name: ";
	cin >> hostName;
	
	cout << "Please enter your nickname!";
	cin >> clientNickname;
	
	//check to see that neither field was left plank
    if(clientNickname == "") 
    { 
		printf("The Nickname Field was left blank!"); 
		exit(1); 
    }
	if(hostName == "")
	{
		printf("The hostname was left blank");
	}
 
    /* get the host */
    if( ( hp = gethostbyname(hostName) ) == NULL ) 
    { 
		printf(" %s Err: unknown host\n",  hostName ); 
		exit(1); 
    } 
    bcopy( hp->h_addr_list[0], (char*)&server_addr.sin_addr, hp->h_length ); 
 
    /* create stream socket */ 
    if( ( socketFileDescriptor = socket( AF_INET, SOCK_STREAM, 0 ) ) == -1 ) 
    { 
		perror( "client: socket failed" ); 
		exit( 1 ); 
    } 
    //connect it 
    if( connect( socketFileDescriptor, (struct sockaddr*)&server_addr, sizeof(server_addr) ) == -1 ) 
    { 
		perror( "client: connect FAILED:" ); 
		exit( 1 ); 
    } 
	
	//connection is succesful, let client know that they have connected to the server correctly
    printf("connect() successful! will send a message to server\n"); 
    printf("Input a string:\n" ); 
	
	//Send the name of the client to the server
	send(socketFileDescriptor, clientNickname, strlen(clientNickname), 0);
	
	/*
	//make sure you can create a thread to read from the server
	if(thread fromServerThread(readFromServer) != 0)//im not 100% sure on this one
	{
		perror("Read thread was not created!");
		shutdown(socketFileDescriptor, SHUT_RDWR);
		exit(1);
	}
	*/
	
	//watch for the ctrl+c interupt
	signal(SIGINT, &SignalHandler);
	
	//send the client's nickname to the server
	send(socketFileDescriptor, clientNickname, strlen(clientNickname), 0);
	
    for(;;)
    { 
		//write the username
		cout << clientNickname << ": " <<endl;
		
		//for the exit command 
		if (strcmp(buf, "/exit") == 0 || strcmp(buf, "/quit") == 0 || strcmp(buf, "/part") == 0)
		{
			cout << "Thank you for using this chatroom" << endl;
			shutdown(socketfd, SHUT_RDWR);
			return 0;
		}
		
		send(socketFileDescriptor, buf, strlen(buf), 0);
    } 
	
	shutdown(socketFileDescriptor, SHUT_RDWR);
    return(0); 
} 