#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include <string>
#include <iostream>
#define PORT 34854

using namespace std;
int main(int argc, char const *argv[]) 
{ 
	int sock = 0;
	struct sockaddr_in serv_addr;
    char buffer[1024] = {0};  
    char *req;
    if (argc !=5 )
    {
        printf("usage: ./client mapid source des size\n");
        return -1; 
    }
    strcpy(req,argv[1]);//mapid
    strcat(req," ");
    strcat(req,argv[2]);//source
    strcat(req," ");
    strcat(req,argv[3]);//des
    strcat(req," ");
    strcat(req,argv[4]);//size
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
		printf("Socket creation error \n"); 
		return -1; 
	} 

	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons(PORT); 
	
	// start exampleCode from geekforgeek 
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
	{ 
		printf("Invalid address\n"); 
		return -1; 
	} 

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
	{ 
		printf("Connection Failed\n"); 
		return -1; 
	} 
	// end exampleCode from geekforgeek 
	printf("The client is up and running\n"); 
	send(sock , req , strlen(req) , 0 ); 
	printf("The client has sent query to AWS using TCP: start vertex %s; destination vertex %s, map %s; file size %s\n",argv[2],argv[3],argv[1],argv[4]); 
	
	read( sock , buffer, 1024); 
	printf("%s",buffer);

    return 0; 
} 
