//============================================================================
// Name        : file_server.cpp
// Author      : Lars Mortensen
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
#include <lib.h>

#define BUF_SIZE 1000

using namespace std;

void sendFile(const char* fileName, int connfd);
int testIfFileExist(const char* fileName);


void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, connfd, portno;
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
//Her oprettes vores socket. SOCK_STREAM er TCP connection. AF_INET betyder det er IPV4 protokollen vi benytter
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
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
//Lyt efter forbindelser, 5 angiver maximum antal forbindelser.
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
//accept blokerer for processen indtil at en klient opretter forbindelse.
     connfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
     if (connfd < 0){ 
     	  close(connfd);
     	  close(sockfd);
          error("ERROR on accept");
     }
//igen sættes en buffer til 0
     bzero(buffer,256);

//forbindelsen er nu oprettet. Hvilken fil skal sendes?
//read() blokerer processen til vi har modtaget en besked fra klienten
     if (read(connfd,buffer,255) < 0){
     		close(connfd);
     		close(sockfd);
     		error("ERROR reading from socket");
     }
     printf("File to send: %s\n",buffer);


// Test om filen eksisterer 
	 buffer[strlen(buffer)-1]=0; // for at kunne bruge buffer skal vi fjerne null termineringen 
	 if (testIfFileExist(buffer)){
	 		 write(connfd,"\nERROR file does not exist",26);
     		 close(connfd);
     		 close(sockfd);
     		 error("ERROR file does not exist");
	 }
	 else {
	      if (write(connfd,"Ok",2) < 0){
     		 close(connfd);
     		 close(sockfd);
     		 error("ERROR writing to socket");
          }
     }

	 sleep(1);
     sendFile(buffer,connfd);

//Luk forbindelsen
     close(connfd);
     close(sockfd);
     return 0; 
}

int testIfFileExist(const char* fileName){

     FILE *fp = fopen(fileName,"r");

     if(fp==NULL){
			return 1;
     }

	 fclose(fp);
     return 0;
}


void sendFile(const char* fileName, int connfd)
{
        /* Open the file that we wish to transfer */
        FILE *fp = fopen(fileName,"r");
        if(fp==NULL)
        {
            printf("\nError opening file\n");
            return;
        }

        /* Read data from file and send it */
        while(1)
        {
            /* First read file in chunks of BUF_SIZE bytes */
            unsigned char buff[BUF_SIZE]={0};
            int nread = fread(buff,1,BUF_SIZE,fp);
            printf("Bytes read %d \n", nread);

            /* If read was success, send data. */
            if(nread > 0)
            {
                printf("Sending \n");
                write(connfd, buff, nread);
            }

            /*
             * There is something tricky going on with read ..
             * Either there was error, or we reached end of file.
             */
            if (nread < BUF_SIZE)
            {
                if (feof(fp))
                    printf("End of file\n");
                if (ferror(fp))
                    printf("Error reading\n");
                break;
            }
        }
		fclose(fp);
    }	
        

