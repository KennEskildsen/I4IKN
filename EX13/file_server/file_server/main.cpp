#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "Transport.h"

#define BUFSIZE 1000

using namespace std;

Transport::Transport *transportlayer;

int testIfFileExist(const char* fileName,int length){

	char buf[length];
	memcpy(buf, fileName, length);
	buf[length-1]=0;

    FILE *fp = fopen(buf,"r");

    if(fp==NULL){
		return 0;
    }

	fclose(fp);
    return 1;
}


void sendFile(const char* fileName, int length, Transport::Transport *transportlayer_)
{
        // Fil der ønskes afsendt
        char buf[length];
		memcpy(buf, fileName, length);
		buf[length-1]=0;

        int fd = open(buf, O_RDONLY);
        if(fd == 0)
        {
            printf("\nError opening file\n");
            return;
        }

        // data læses fra filen og afsendes
        while(1)
        {
            // Data brydes op i BUF_SIZE stykker
            char buff[BUFSIZE]={0};

            int nread = read(fd,buff,BUFSIZE);
            printf("Bytes read %d \n", nread);

           // Hvis læsning lykkes afsendes filen
            if(nread > 0)
            {
                printf("Sending \n");
                transportlayer_->send(buff,nread);
            }
			else
			{
            printf("File sent \n");
            break;
            }
        }
    }	
        

int main(int argc, char** argv)
{
    char buffer[BUFSIZE];
	int n;

	transportlayer = new Transport::Transport(BUFSIZE);

//Hvilken fil skal sendes?
	n = transportlayer->receive(buffer, BUFSIZE);
    printf("File to send: '%s'\n",buffer);
	
// Test om filen eksisterer 
	if (testIfFileExist(buffer,n)==0)
	{
		 char buf[]="ERROR file does not exist\n";
		 transportlayer->send(buf,26);
 		 cout<<"ERROR file does not exist"<<endl;
 		 exit(1);
	 }
	 else 
	 {
	    char buf[]="Ok";
		transportlayer->send(buf,2);    
     }

	 sleep(1);
     sendFile(buffer,n,transportlayer);

//Luk forbindelsen
     return 0; 
}