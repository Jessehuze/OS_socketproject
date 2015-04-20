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
#include <cstdlib>
#include <csignal>
#include <sys/types.h> 
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/socket.h>  // define socket
#include <netinet/in.h>  // define internet socket
#include <netdb.h>       // define internet socket
#include <thread>
using namespace std;
 
#define SERVER_PORT 3932     // define a server port number 
 
 //for the handling of ctrl+c
 void interruptHandler(int sig);
  
int main() 
{ 
    int socketFileDescriptor; 
    char buf[512]; 
	char clientNickname[64];
	char clientRemove[64];
	char hostName[64];
	thread fromServerThread; // waits for input from server and prints it to the screen
	signal(SIGINT, &interruptHandler);//for interupt handling
	
	
	//Get the server and client nickname
	cout << "Please enter a server name: ";
	cin >> hostName;
	
	cout << "Please enter your nickname!";
	cin >> clientNickname;
	
	//check to see that neither field was left plank
	//check that the nicknamefield isnt blank
    if(strlen(clientNickname) == 0) 
    { 
		printf("The Nickname Field was left blank!"); 
		exit(1); 
    }
	//check to make sure the hostname wasnt left blank
	if(strlen(hostName) == 0)
	{
		printf("The hostname was left blank");
		exit(1);
	}
	
	//get ready to connect the server
	struct serverSockAddr = {AF_INET, htons(port)};
	struct hostent* hp; 
	
    /* get the host */
	//if( ( hp = gethostbyname(hostName) ) == NULL )
    if( (gethostbyname(hostName) ) == NULL ) 
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
    if( connect( socketFileDescriptor, (struct serverSockAddr*)&server_addr, sizeof(server_addr) ) == -1 ) 
    { 
		perror( "client: connect FAILED:" ); 
		exit( 1 ); 
    } 
	
	//connection is succesful, let client know that they have connected to the server correctly
    printf("connect() successful! will send a message to server\n"); 
    printf("Input a string:\n" ); 
	
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
			shutdown(socketFileDescriptor, SHUT_RDWR);
			return 0;
		}
		
		send(socketFileDescriptor, buf, strlen(buf), 0);
    } 
	
	shutdown(socketFileDescriptor, SHUT_RDWR);
    return(0); 
} 


void interruptHandler(int sig)
 {
	if(SIGINT)
	{
		cout << "Please type /exit , /quit , or /part to leave the chat room" << endl;
	}
	return;
 }