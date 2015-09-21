//============================================================================
// Name        : file_client.cpp
// Author      : I4IKN Gruppe 8
// Version     : 1.0
// Description : file_client in C++
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
#include <lib.h>

// Buffer til modtagelse
#define BUF_SIZE 1000

using namespace std;

// funktion til at modtage fil, tager (filnavn og Socket_file_descriptor som argumenter)
void receiveFile(const char* fileName, int socketfd);

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
	// file recieve: 	sti til fil der ønskes modtaget	
    char file_rc[256];
	// file locate: 	sti til hvor filen ønskes gemt
    char file_lc[256];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
	// Konverter fra ascii til int 
    portno = atoi(argv[2]);
	// Sættes lig serveren 
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
	// Serveren IP-adresse
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
	// Sætter server-adressen til 0
    bzero((char *) &serv_addr, sizeof(serv_addr));
	// IPV4
    serv_addr.sin_family = AF_INET;
	// Konfigurer server-adressen (næsten samme som i serveren) bcopy kopierer en karakterstring void bcopy(char *s1, char *s2, int length)
    bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);
    serv_addr.sin_port = htons(portno);
	// Her connectes til serveren
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");

	// Hvilken fil ønskes modtaget
    printf("Please enter witch file to recive from server: ");
    bzero(file_rc,256);
    fgets(file_rc,255,stdin);
	// Hvor skal den gemmes
    printf("Please enter where to locate the file: ");
    bzero(file_lc,256);
    scanf("%s",file_lc);
	// Skriv til socket hvilken fil vi ønsker at hente
	n = write(sockfd,file_rc,strlen(file_rc));
    if (n < 0) {
   		 close(sockfd);
         printf("ERROR writing to socket");
         exit(1);
         }
	// Vent på svar fra socket om filen eksisterer
    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    if (n < 0){
    	 close(sockfd);
         printf("ERROR reading from socket");
         exit(1);
         }	 
    printf("%s\n",buffer);
  
	if (!((buffer[0] == 'O') && (buffer[1]=='k'))){
    	close(sockfd);
    	printf("\nERROR file does not exist on server\n");
    	exit(1);
    	}
	// Hent filen 
    receiveFile(file_lc,sockfd);
	// Luk connection 
    close(sockfd);
    return 0;
}


void receiveFile(const char fileName[], int sockfd)
{
	// Opret en fil hvor dataen vil blive modtaget
    FILE *fp = fopen(fileName, "w");
    if(NULL == fp)
    {
        printf("Error opening file");
        return;
    }

    // Modtag data i stykker af BUF_SIZE i bytes
    int bytesReceived = 0;
    char buff[BUF_SIZE];
    memset(buff, '0', sizeof(buff));
    while((bytesReceived = read(sockfd, buff, BUF_SIZE)) > 0)
    {
        printf("Bytes received %d\n",bytesReceived);
        fwrite(buff, 1,bytesReceived,fp);
    }

    if(bytesReceived < 0)
    {
        printf("\n Read Error \n");
    }

     printf("File copied\n");

}

