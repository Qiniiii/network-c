#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include<iostream>
#include<fstream>

using namespace std;

#define PORT 31854 
#define MAXLINE 1024 

// Driver code 
int main() { 
	int sockfd; 
	char buffer[MAXLINE]; 
	struct sockaddr_in servaddr, clientAdd; 
	
	// Creating socket file descriptor 
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	memset(&servaddr, 0, sizeof(servaddr)); 
	memset(&clientAdd, 0, sizeof(clientAdd)); 
	
	// Filling server information 
	servaddr.sin_family = AF_INET; // IPv4 
	servaddr.sin_addr.s_addr = INADDR_ANY; 
	servaddr.sin_port = htons(PORT); 
	
	// Bind the socket with the server address 
	if ( bind(sockfd, (const struct sockaddr *)&servaddr, 
			sizeof(servaddr)) < 0 ) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 
	printf("The Server B is up and running using UDP on port 31854\n");

	while(1){
		unsigned int len, n; 
		len = sizeof(clientAdd); //len is value/resuslt 
		int fou=0;
		n = recvfrom(sockfd, (char *)buffer, MAXLINE, 
					MSG_WAITALL, ( struct sockaddr *) &clientAdd, 
					&len); 
		buffer[n] = '\0'; 
		printf("The Server B has received input for finding graph of map %s\n",buffer);

		//read map
		ifstream myMap;
		myMap.open("./map2.txt");
		char line[100];
		char graph[1024];
		if (myMap.is_open()) {
			while (!myMap.eof()) {
				myMap.getline(line,100);
				char tem1[100];
				strcpy(tem1,buffer);
				if(!strcmp(tem1,line)){
					myMap.getline(line,100);
					strcpy(graph,tem1);
					while(!myMap.eof() && strlen(line)>1){
						strcat(graph,"\n");
						strcat(graph,line);
						myMap.getline(line,100);
					}
					sendto(sockfd, (const char *)graph, strlen(graph), 
						MSG_CONFIRM, (const struct sockaddr *) &clientAdd, 
							len); 
					printf("The Server B has sent Graph to AWS\n");
					fou=1;
					break;
				}
			} 
		}
		myMap.close();
		//end read file
		if(!fou){
			printf("The Server B does not have the required graph id %s\n",buffer); 
			sendto(sockfd, "0", 1, 
				MSG_CONFIRM, (const struct sockaddr *) &clientAdd, 
					len); 
			printf("The Server B has sent \"Graph not Found\" to AWS\n"); 
		}
		
		 
	}
	return 0;
} 
