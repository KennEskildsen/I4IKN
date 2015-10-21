//============================================================================
// Name        : file_udp_server.cpp
// Author      : Mr. Awesome from Royal-Randers 
// Version     : 1.0
// Description : file_server in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "lib.h"

#define BUF_SIZE 1000

using namespace std;


int main(int argc, char *argv[])
{
     int sockfd, connfd, portno;
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     socklen_t addrlen = sizeof(cli_addr);            /* length of addresses */
     int n;
     int recvlen;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
//Her oprettes vores socket. SOCK_DGRAM er UDP connection. AF_INET betyder det er IPV4 protokollen vi benytter
     sockfd = socket(AF_INET, SOCK_DGRAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
//bzero sætter alle værdier i en buffer til 0
     bzero((char *) &serv_addr, sizeof(serv_addr));
//atoi konverterer ascii to integer. 
     portno = atoi(argv[1]);
//sin_family er hvilken type forbindelse der er, her IPV4
//sin_addr.s_addr sættes den ipadressen på serveren INADDR_ANY henter den selv 
//sin_port sættes til vores portno, men den skal konverteres til network-byte-order først htons()
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
//bind binder adressen til serveren. Ved fejl er adressen sikkert allerede brugt 
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0){ 
              close(sockfd);
              error("ERROR on binding");
     }

     bzero(buffer,256);

     for (;;) {
         printf("waiting on port %d\n", PORT);
         recvlen = recvfrom(sockfd, buffer, BUFSIZE, 0, (struct sockaddr *)&cli_addr, &addrlen);
         printf("received %d bytes\n", recvlen);
         if (recvlen > 0) 
         {
                 buffer[recvlen] = 0;
                 printf("received message: \"%s\"\n", buffer);
         }

         if(buffer[0]=='u')
         {

         	      /* Open the file that we wish to transfer */
                 FILE *fp = fopen("/proc/uptime","r");
                 if(fp==NULL)
                 {
                     printf("\nError opening file\n");
                     exit(1);
                 }
                 unsigned char buff[BUF_SIZE]={0};
                 int nread = fread(buff,1,BUF_SIZE,fp);
                 printf("Bytes read from file %d \n", nread);

         	   if (sendto(sockfd, buff, nread, 0, (struct sockaddr *)&cli_addr, addrlen)==-1)
         	   {
          			 fprintf(stderr, "ERROR in sendto\n");
           			 exit(1);
      		   }
		 }
      		   
           if(buffer[0]=='l')
         {

         	      /* Open the file that we wish to transfer */
                 FILE *fp = fopen("/proc/loadavg","r");
                 if(fp==NULL)
                 {
                     printf("\nError opening file\n");
                     exit(1);
                 }
                 unsigned char buff[BUF_SIZE]={0};
                 int nread = fread(buff,1,BUF_SIZE,fp);
                 printf("Bytes read from file %d \n", nread);

         	   if (sendto(sockfd, buff, nread, 0, (struct sockaddr *)&cli_addr, addrlen)==-1)
         	   {
          			 fprintf(stderr, "ERROR in sendto\n");
           			 exit(1);
      		   }
		 }
	 }
         

 	close(sockfd);
}	




