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
 
#include <stdio.h> 
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
 void SignalHandler(int sig, siginfo_t *si, void *context)
 {
	if(SIGINT)
	{
		cout << "Please type /exit , /quit , or /part to leave the chat room" << endl;
	}
	return;
 }
 
 
int main() 
{ 
    int socketFileDescriptor; 
    struct sockaddr_in server_addr = { AF_INET, htons( SERVER_PORT ) }; 
    char buf[512]; 
    struct hostent *hp; 
	thread fromServerThread; // waits for input from server and prints it to the screen
	string clientNickname = "";
	string hostName="";
	//for interupt handling
	struct sigAction sVal;

	//show that we are using a dfiferent signal handler which takes 3 args
	sval.sa_flags = SA_SIGINFO;
	sval.sa_sigaction = HandleSignal;
	
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
		printf(" %s unknown host\n",  hostName ); 
		exit(1); 
    } 
    bcopy( hp->h_addr_list[0], (char*)&server_addr.sin_addr, hp->h_length ); 
 
    /* create a socket */ 
    if( ( socketFileDescriptor; = socket( AF_INET, SOCK_STREAM, 0 ) ) == -1 ) 
    { 
		perror( "client: socket failed" ); 
		exit( 1 ); 
    } 
 
    /* connect a socket */ 
    if( connect( socketFileDescriptor;, (struct sockaddr*)&server_addr, 
		 sizeof(server_addr) ) == -1 ) 
    { 
		perror( "client: connect FAILED:" ); 
		exit( 1 ); 
    } 
	
    printf("connect() successful! will send a message to server\n"); 
    printf("Input a string:\n" ); 
	
	//send the clients name to the server
	char myName[ ] = clientNickname.c_str();
	write(socketFileDescriptor, myName, sizeof(myName));
	
    while( gets(buf) != NULL) 
    { 
		write(socketFileDescriptor, buf, sizeof(buf));
		read(socketFileDescriptor, buf, sizeof(buf)); 
		printf("SERVER ECHOED: %s\n", buf); 
		
		//write if to exit the server
		
		//write if for the handling of ctrl + c
    } 
	
	//get ready to exit the program
	pthread_exit(fromServerThread);
    close(socketFileDescriptor;); 
    return(0); 
} 