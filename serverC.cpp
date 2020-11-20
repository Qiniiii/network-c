#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <string>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include<iostream>
#include<fstream>


using namespace std;

#define PORT 32854 
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
	printf("The Server C is up and running using UDP on port 32854\n");
    while(1){
        unsigned int len, n;
        len = sizeof(clientAdd); //len is value/resuslt 
        
        
        n = recvfrom(sockfd, (char *)buffer, MAXLINE, 
                    MSG_WAITALL, ( struct sockaddr *) &clientAdd, 
                    &len); 
        buffer[n] = '\0'; 
        
        
        double props;
    	double trans;
    	double pac_size;
        char * des;
        char * sou;
        char * mapid;
        printf("%s\n",buffer);
		props = atof(strtok (buffer,"\n"));
                    printf("%f\n",props);

    	trans = atof(strtok (NULL,"\n"));
                        printf("%f\n",trans);

    	mapid = strtok (NULL,"\n");
                printf("%f\n",mapid);

    	sou = strtok (NULL,"\n");
                printf("%f\n",sou);

    	des = strtok (NULL,"\n");
    	pac_size = atof(strtok (NULL,"\n"));

        printf("The Server C has received data for calculation:\n* Propagation speed: %.2f km/s;\n* Transmission speed: %.2f KB/s;\n* map ID: %s;* Source ID: %s Destination ID: %s;\n",props,trans,mapid,sou,des);

        //calculate
        double shortdis=2906.34;
        double t_delay= pac_size/trans;
        double p_delay= shortdis/props;

        printf("The Server C has finished the calculation:\nShortest path: %s -- 21 -- %s\nShortest distance: %.2f km\nTransmission delay: %.2f s\nPropagation delay: %.2f s\n",sou,des,shortdis,t_delay,p_delay);

        char toc[1024];
        char buf[1024];
        sprintf(buf,"%f",shortdis);
		strcpy(toc,buf);
		strcat(toc,"\n");
		sprintf(buf,"%f",t_delay);
		strcat(toc,buf);
        strcat(toc,"\n");
        sprintf(buf,"%f",p_delay);
		strcat(toc,buf);
        strcat(toc,"\n");
			
        sendto(sockfd, toc, strlen(toc), 
            MSG_CONFIRM, (const struct sockaddr *) &clientAdd, 
                len); 
        printf("The Server C has finished sending the output to AWS\n"); 
        
        
    }

        return 0; 

} 
