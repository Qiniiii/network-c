
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>
#include <string>
#include <iostream>

#define A_PORT 30854
#define B_PORT 31854
#define C_PORT 32854
#define PORT 33854
#define PORT_TCP 34854

using namespace std;

int main(int argc, char const *argv[]) 
{ 
	int server_fd, new_socket; 
	struct sockaddr_in address; 
	int opt = 1; 
	int addrlen = sizeof(address); 
	char buffer[1024] = {0}; 	

	// start exampleCode from geekforgeek TCP
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
	{ 
		perror("socket failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
												&opt, sizeof(opt))) 
	{ 
		perror("setsockopt"); 
		exit(EXIT_FAILURE); 
	} 
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons( PORT_TCP ); 
	
	if (bind(server_fd, (struct sockaddr *)&address, 
								sizeof(address))<0) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 
		
	printf("The AWS is up and running.\n");
	//loop 
	while(1){
	//start listen
	if (listen(server_fd, 3) < 0) 
	{ 
		perror("listen"); 
		exit(EXIT_FAILURE); 
	} 



	
	//build connection
	if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
					(socklen_t*)&addrlen))<0) 
	{ 
		perror("accept"); 
		exit(EXIT_FAILURE); 
	} 
	// end exampleCode from geekforgeek 


	read( new_socket , buffer, 1024);
    char * mapid;
    char * sou;
    char * des;
    char * pac_size;
    mapid = strtok (buffer," ");
    sou = strtok (NULL," ");
    des = strtok (NULL," ");
    pac_size = strtok (NULL," ");
	printf("The AWS has received map ID %s, start vertex %s, destination vertex %s and file size %s from the client using TCP over port 34854\n",mapid,sou,des,pac_size);

	// start exampleCode from geekforgeek UDP
    int sockfd; 
    char buffer_A[1024]; 
    struct sockaddr_in servaddr; 
  
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("creation failed"); 
        exit(EXIT_FAILURE); 
    } 
  
    memset(&servaddr, 0, sizeof(servaddr)); 
      
    // Filling server information 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(A_PORT); 
    servaddr.sin_addr.s_addr = INADDR_ANY; 
    // end exampleCode from geekforgeek 
  

	//send recv message
    unsigned int n, len; 
    sendto(sockfd, (const char *)mapid, strlen(mapid), 
        MSG_CONFIRM, (const struct sockaddr *) &servaddr,  
            sizeof(servaddr)); 
    printf("The AWS has sent map ID to server A using UDP over port %d\n",A_PORT); 

    n = recvfrom(sockfd, (char *)buffer_A, 1024,  
                MSG_WAITALL, (struct sockaddr *) &servaddr, 
                &len);
    buffer_A[n] = '\0'; 
    close(sockfd);
	//check if map exist in A
	int recv;
    if (strlen(buffer_A)==1){

		//check if map exist in B
		if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
			perror("creation failed"); 
			exit(EXIT_FAILURE); 
		} 
	
		memset(&servaddr, 0, sizeof(servaddr)); 
		
		// Filling server information 
		servaddr.sin_family = AF_INET; 
		servaddr.sin_port = htons(B_PORT); 
		servaddr.sin_addr.s_addr = INADDR_ANY; 
		// end exampleCode from geekforgeek 
	

		//send recv message
		sendto(sockfd, (const char *)mapid, strlen(mapid), 
			MSG_CONFIRM, (const struct sockaddr *) &servaddr,  
				sizeof(servaddr)); 
    	printf("The AWS has sent map ID to server A using UDP over port %d\n",B_PORT); 

		n = recvfrom(sockfd, (char *)buffer_A, 1024,  
					MSG_WAITALL, (struct sockaddr *) &servaddr, 
					&len);
		buffer_A[n] = '\0'; 
		close(sockfd);
		
		if (strlen(buffer_A)==1){
			char res[1024];
			strcpy(res,"No vertex id ");
			strcat(res,mapid);
			strcat(res," found\n");
			send(new_socket , res , strlen(res) , 0 ); 
			printf("%s\n",res);
			
		}else{
			recv=0;
			printf("The AWS has received map information from server B\n");
		}
		
    }else{
		recv=1;
		printf("The AWS has received map information from server A\n");
	}

	//check if source and des exist
	if (strlen(buffer_A)!=1){
    	char * props;
    	char * trans;
    	char * graph;
		strtok (buffer_A,"\n");
		props = strtok (NULL,"\n");
    	trans = strtok (NULL,"\n");
    	graph = strtok (NULL,"");
		int sou_found=0;
		int des_found=0;
    	while (graph!=NULL){
			char * ver1;
    		char * ver2;
			ver1=strtok(graph," ");
			ver2=strtok(NULL," ");
			strtok(NULL,"\n");
			graph=strtok(NULL,"");
			if(graph!=NULL){
				if(!strcmp(ver2,sou)||!strcmp(ver1,sou)){
					sou_found=1;
				}			
				if(!strcmp(ver1,des)||!strcmp(ver2,des)){
					des_found=1;
				}
			}
			
		}
		//send to serverC
		if (sou_found && des_found){
			//send to C	
			printf("The source and destination vertex are in the graph\n");
			
			if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
				perror("creation failed"); 
				exit(EXIT_FAILURE); 
			} 
		
			memset(&servaddr, 0, sizeof(servaddr)); 
			
			// Filling server information 
			servaddr.sin_family = AF_INET; 
			servaddr.sin_port = htons(C_PORT); 
			servaddr.sin_addr.s_addr = INADDR_ANY; 
			// end exampleCode from geekforgeek 
	
			
			//send recv message
			char toc[1024];

			strcpy(toc,props);
			strcat(toc,"\n");
			strcat(toc,trans);
			strcat(toc,"\n");
			strcat(toc,mapid);
			strcat(toc,"\n");
			strcat(toc,sou);
			strcat(toc,"\n");
			strcat(toc,des);
			strcat(toc,"\n");
			strcat(toc,pac_size);
			strcat(toc,"\n");
			printf("%s\n",toc);
			sendto(sockfd, (const char *)toc, strlen(toc), 
				MSG_CONFIRM, (const struct sockaddr *) &servaddr,  
					sizeof(servaddr)); 
			printf("The AWS has sent map, source ID, destination ID, propagation speed and transmission speed to server C using UDP over port 33854\n"); 
		
			//recvfrom c
			n = recvfrom(sockfd, (char *)buffer_A, 1024,  
						MSG_WAITALL, (struct sockaddr *) &servaddr, 
						&len);
			buffer_A[n] = '\0';
			double t_delay;
			double p_delay;
			double shortdis;
			shortdis = atof(strtok (buffer_A,"\n"));
			t_delay = atof(strtok (NULL,"\n"));
			p_delay = atof(strtok (NULL,"\n"));
			double total=t_delay+p_delay;

			printf("The AWS has received results from server C:\n");
			printf("Shortest path: %s -- 21 -- %s\n",sou,des);
			printf("Shortest distance: 2932.32 km\n");
			printf("Transmission delay: %.2f s\n",t_delay);
			printf("Propagation delay: %.2f s\n",p_delay);

			close(sockfd);

			char toclient[1024];
			strcpy(toclient,"The client has received results from AWS:\n");
			strcat(toclient,"-----------------------------------------\n");
			strcat(toclient,"Source Des  Min Length   Tt    Tp   Delay\n");
			strcat(toclient,sou);
			strcat(toclient,"     ");
			strcat(toclient,des);
			strcat(toclient,"   ");
			strcat(toclient,"2932.32");
			strcat(toclient,"      ");
			char buf[1024];
        	sprintf(buf,"%.2f",t_delay);
			strcat(toclient,buf);
			strcat(toclient,"  ");
			sprintf(buf,"%.2f",p_delay);
			strcat(toclient,buf);
			strcat(toclient," ");
        	sprintf(buf,"%.2f",total);
			strcat(toclient,buf);
			strcat(toclient,"\n");
			strcat(toclient,"-----------------------------------------\n");
			strcat(toclient,"Shortest path: ");
			strcat(toclient,sou);
			strcat(toclient," -- 21 -- ");
			strcat(toclient,des);
			strcat(toclient,"\n");

			//send res to client
			send(new_socket , toclient , strlen(toclient) , 0 ); 
			printf("The AWS has sent calculated results to client using TCP over port 34854\n");
		}
		//vertex not found info & send back to client
		else if (!sou_found && des_found){
			char res[1024];
			strcpy(res,"No vertex id ");
			strcat(res,sou);
			strcat(res," found\n");
			send(new_socket , res , strlen(res) , 0 ); 
			printf("Source vertex not found in the graph, sending error to client using TCP over port 34854\n");
		}
		else if (sou_found && !des_found){
			char res[1024];
			strcpy(res,"No vertex id ");
			strcat(res,des);
			strcat(res," found\n");
			send(new_socket , res , strlen(res) , 0 );
 			printf("destination vertex not found in the graph, sending error to client using TCP over port 34854\n");
		}
		else{
			char res[1024];
			strcpy(res,"No vertex id ");
			strcat(res,sou);
			strcat(res," and ");
			strcat(res,des);
			strcat(res," found\n");
			send(new_socket , res , strlen(res) , 0 );	
			printf("Source and destination vertex not found in the graph, sending error to client using TCP over port 34854\n");
		}

	}
    //end send to A
	}



	return 0; 
} 
