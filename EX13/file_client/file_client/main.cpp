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
#include "Transport.h"

#define BUFSIZE 1000

using namespace std;

Transport::Transport *transportlayer;

void receiveFile(const char fileName[], Transport::Transport *transportlayer_)
{
	// Opret en fil hvor dataen vil blive modtaget
    FILE *fp = fopen(fileName, "w");
    if(NULL == fp)
    {
        cout<<"Error opening file";
        return;
    }

    // Modtag data i stykker af BUF_SIZE i bytes
    int bytesReceived = 0;
    char buff[BUFSIZE];
    memset(buff, '0', sizeof(buff));
    while((bytesReceived = transportlayer_->receive(buff,BUFSIZE) > 0))
    {
        cout<<"Bytes received: "<<bytesReceived<<endl;
        fwrite(buff, 1,bytesReceived,fp);
    }

    if(bytesReceived < 0)
    {
        cout<<"\n Read Error \n";
    }

     cout<<"File copied\n";
}

int main(int argc, char** argv)
{
	transportlayer = new Transport::Transport(BUFSIZE);
	int n;
    char buffer[BUFSIZE];
    char file_rc[BUFSIZE];
    char file_lc[BUFSIZE];

    while(1)
    {
		// Hvilken fil ønskes modtaget
	    printf("Please enter witch file to recive from server: ");
	    bzero(file_rc,256);
	    fgets(file_rc,255,stdin);
		// Hvor skal den gemmes
	    printf("Please enter where to locate the file: ");
	    bzero(file_lc,256);
	    scanf("%s",file_lc);
		// Skriv til server hvilken fil vi ønsker at hente
		transportlayer->send(file_rc,strlen(file_rc));
		// Vent på svar fra server om filen eksisterer
	    bzero(buffer,256);
	    n = transportlayer->receive(buffer,BUFSIZE);
	    if (n < 0){
	    	 printf("ERROR reading from server");
	         exit(1);
	         }	 
	    printf("%s\n",buffer);
	  
		if (!((buffer[0] == 'O') && (buffer[1]=='k'))){
	    	printf("\nERROR file does not exist on server\n");
	    	exit(1);
	    	}
		// Hent filen 
	    receiveFile(file_lc,transportlayer);
    }

	return 0;
}

