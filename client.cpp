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
 
#define SERVER_PORT 3934     // define a server port number 
 
 //for the handling of ctrl+c
 void interruptHandler(int sig);
 
 //for the server handling
 void* readServerFeedback(void* dmyptr);
 
 //because it has to be global for visiblity to the readServerFeedback Function
 int socketFileDescriptor; 
 
int main() 
{ 
    char buf[512]; 
	char clientNickname[64];
	char clientRemove[64];
	char hostName[64];
	signal(SIGINT, &interruptHandler);//for interupt handling
	struct sockaddr_in serverAddr;//Server Address
	
	//Get the server and client nickname
	cout << "Please enter a server name: ";
	cin >> hostName;
	
	cout << "Please enter your nickname: ";
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
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	
	struct hostent* hp; 
    /* get the host */
	//if( ( hp = gethostbyname(hostName) ) == NULL )
    if( (hp = gethostbyname(hostName) ) == NULL ) 
    { 
		printf(" %s Err: unknown host\n",  hostName ); 
		exit(1); 
    } 
    
    bcopy( (char*)hp->h_addr, (char*) &serverAddr.sin_addr.s_addr, hp->h_length ); 
    
 
    /* create stream socket */ 
    if( ( socketFileDescriptor = socket( AF_INET, SOCK_STREAM, 0 ) ) == -1 ) 
    { 
		perror( "client: socket failed" ); 
		exit( 1 ); 
    } 
    //connect it 
    if( connect( socketFileDescriptor, (struct sockaddr*) &serverAddr, sizeof(serverAddr) ) == -1 ) 
    { 
		perror( "client: connect FAILED:" ); 
		exit( 1 ); 
    } 
	
	//connection is succesful, let client know that they have connected to the server correctly
    printf("connect() successful! will send a message to server\n"); 
    printf("Input a string:\n" ); 
	
	//send the client's nickname to the server
	send(socketFileDescriptor, clientNickname, strlen(clientNickname), 0);
	
	//prepare for input from the server
	pthread_t fromServerThread;
	if(pthread_create(&fromServerThread, NULL, readServerFeedback, NULL) != 0)
	{
		perror("Unable to read fromServerThread!");
		shutdown(socketFileDescriptor, SHUT_RDWR);
		exit(1);
	}
	
    while (true)
    { 
		//write the username
		cout << clientNickname << ": " ;
		//get the messages
    //cin >> buf;
		cin.getline(buf, sizeof(buf));
		
		//for the exit command 
		if (strcmp(buf, "/exit") == 0 || strcmp(buf, "/quit") == 0 || strcmp(buf, "/part") == 0)
		{
			cout << "Thank you for using this chatroom" << endl;
			shutdown(socketFileDescriptor, SHUT_RDWR);
			return 0;
		}
		
		//send the message
		send(socketFileDescriptor, buf, strlen(buf), 0);
		
		//clear the buffer for the next message
		memset(buf, '\0', 512);
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

 
 void* readServerFeedback(void* dmyptr)
{
	bool connectionOpen = true;
	char lebeuof[256];
	memset(lebeuof, '\0', 255);
	char otherClientName[64];
	char clearClientName[64];
	
	
	while(read(socketFileDescriptor, lebeuof, 255));
	{
		if(strlen(lebeuof) == 8 && strcmp(lebeuof, "* EXIT *"))
		{
			strcpy(lebeuof, "SERVER IS NOW CLOSING THE CONECTION!!!");
			connectionOpen = false;
		}
	
		//show the name and clear it afterward
		cout << clearClientName << lebeuof << endl;
		cout << otherClientName << ": " << flush;
		
		//for when the connection is done wait 10, remove the name then quit the program
		if(connectionOpen = false)
		{
			shutdown(socketFileDescriptor, SHUT_RDWR);
			sleep(10);
			cout << clearClientName << endl;
			
			exit(0);
		}
		memset(lebeuof, '\0', 255);
	}
	//thread is done being used
	pthread_exit(NULL);
}