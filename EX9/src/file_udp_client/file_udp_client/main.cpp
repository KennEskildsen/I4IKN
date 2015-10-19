//============================================================================
// Name        : file_client.cpp
// Author      : Lars Mortensen
// Version     : 1.0
// Description : file_client in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "lib.h"

#define BUFLEN 1000
#define NPACK 10

using namespace std;

int main(int argc, char *argv[])
{
  	struct sockaddr_in si_other;
  	int s, i, portno,recvlen, slen=sizeof(si_other);
  	char buf[BUFLEN];

    char my_message[8];

    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(1);
    }

	// konverter ascii til integer
    portno = atoi(argv[2]);
    socklen_t addrlen = sizeof(argv[1]); 

     if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
       		fprintf(stderr,"ERROR in socket\n");

      memset((char *) &si_other, 0, sizeof(si_other));
      si_other.sin_family = AF_INET;
      si_other.sin_port = htons(PORT);
      if (inet_aton(argv[1], &si_other.sin_addr)==0) {
         fprintf(stderr, "inet_aton() failed\n");
         exit(1);
      }
         
    printf("Client connected. Please enter U/u for uptime or L/l for loadavg: ");
    bzero(my_message,256);
    scanf("%s",my_message);

    if(my_message[0] == 'U' || my_message[0] == 'u' )
    		my_message[0] = 'u';
    else if(my_message[0] == 'l' || my_message[0] == 'L')
    		my_message[0] = 'l';
    else
		{
		fprintf(stderr,"Error in input\n");
		exit(1);
		}

    printf("Sending message %d\n", i);
       if (sendto(s, my_message, strlen(my_message)+1, 0, (struct sockaddr *)&si_other, slen)==-1)
       {
           fprintf(stderr, "ERROR in sendto\n");
           exit(1);
       }

	printf("waiting for data %d\n", PORT);
    recvlen = recvfrom(s, buf, BUFSIZE, 0, (struct sockaddr *)&si_other,&addrlen );
    printf("received %d bytes\n", recvlen);
    printf("received: %s\n", buf);

      close(s);
      return 0;
    }

