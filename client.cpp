// Jacob Gallow and Jesse Hughes
//CS 3800
//client.cpp
//implements the logic for the client function

#include <iostream> 
#include <cstdlib>
#include <csignal>
#include <cstring>
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

//FUNCTION PROTOTYPES--------------------------------------------------------------------------------------------
//for the handling of ctrl+c
void interruptHandler(int sig);
//for the server handling
void* readServerFeedback(void* dmyptr);
//END PROTOTYPES ----------------------------------------------------------------------------------------------------

//GLOBAL VARIABLES FOR CROSS FUNCTION VISIBLITY ------------------------------------------------------ 
#define SERVER_PORT 3936     // define a server port number 
int socketFileDescriptor; 
char clientNickname[64];
char clientNameRemove[64];
pthread_t fromServerThread;
//END GLOBAL VARIABLES----------------------------------------------------------------------------------------------

int main() 
{ 
    char hostName[64];
    signal(SIGINT, &interruptHandler);//for interupt handling
    struct sockaddr_in serverAddr;//Server Address
	serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    struct hostent* hp; 
	
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
	  	
    /* get the host */
    if( (hp = gethostbyname(hostName) ) == NULL ) 
    { 
		printf(" %s Err: unknown host\n",  hostName ); 
		exit(1); 
    } 
    bcopy( (char*)hp->h_addr, (char*) &serverAddr.sin_addr.s_addr, hp->h_length ); 
    
     /* create stream socket */ 
    if( ( socketFileDescriptor = socket( AF_INET, SOCK_STREAM, 0 ) ) == -1 ) 
    { 
		perror( "client: create socket failed" ); 
		exit( 1 ); 
    } 
    //connect it 
    if( connect( socketFileDescriptor, (struct sockaddr*) &serverAddr, sizeof(serverAddr) ) == -1 ) 
    { 
		perror( "client: connect FAILED" ); 
		exit( 1 ); 
    } 

	if(pthread_create(&fromServerThread, NULL, readServerFeedback, NULL) != 0)
	{
		perror("Unable to create the fromServerThread!");
		close(socketFileDescriptor);
		exit(1);
	}
		
	//connection is succesful, let client know that they have connected to the server correctly
    printf("connect() successful! will send a message to server\n"); 
    printf("Type a message to broadcast it \n" ); 
    
	char buf[512]; 
    
	//send the client's nickname to the server
	send(socketFileDescriptor, clientNickname, strlen(clientNickname), 0); //DIFF BETWEEN WRITE AND SEND???
    
	while (true)
    { 
		//write the username
		cout << clientNickname << ": " ;
		//get the messages
		cin.getline(buf, sizeof(buf)); //is it flawed to use a getline in this
		
		//for the exit command 
		if (strcmp(buf, "/exit") == 0 || strcmp(buf, "/quit") == 0 || strcmp(buf, "/part") == 0)
		{
			cout << "Thank you for using this chatroom" << endl;
			send(socketFileDescriptor, buf, strlen(buf), 0);
			close(socketFileDescriptor);
			exit ( 0 );
		}
		
		//send the message
		send(socketFileDescriptor, buf, strlen(buf), 0);
		
		//clear the buffer for the next message
		memset(buf, '\0', 512);
    } 
	
	close(socketFileDescriptor);
    return(0); 
}


//FUNCTION DEFININTIONS---------------------------------------------------------------------------------------------

//Handling for when user inputs ctrl+c as an attempt to exit
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
	
	while(read(socketFileDescriptor, lebeuof, 255));
	{
		//for when the connection is done wait 10, remove the name then quit the program
		if(strlen(lebeuof) == 8 && strcmp(lebeuof, "* EXIT *"))
		{
			strcpy(lebeuof, "SERVER IS NOW CLOSING THE CONECTION!!!");
			connectionOpen = false;
			
			close(socketFileDescriptor);
			sleep(10);
			cout << clientNameRemove << endl;
			
			exit(0);
		}
	
		//show the name and clear it afterward
		cout << clientNameRemove << lebeuof << endl;
		cout << clientNickname << ": " << flush;
		
		memset(lebeuof, '\0', 255);
	}
	//thread is done being used
	pthread_exit(NULL);
}

//ENDFUNCTION DEFINITIONS---------------------------------------------------------------------------------------------