#include "Link.h"

#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */

Link::Link(int bufsize)
{
	buffer = new char[(bufsize*2)+2];
	
    serialPort=v24OpenPort("/dev/ttyS1",V24_STANDARD);
    if ( serialPort==NULL )
    {
        fputs("error: sorry, open failed!\n",stderr);
        exit(1);
    }

    int rc=v24SetParameters(serialPort,V24_B115200,V24_8BIT,V24_NONE);
    if ( rc!=V24_E_OK )
    {
        fputs("error: setup of the port failed!\n",stderr);
        v24ClosePort(serialPort);
        exit(1);
    }

}


void Link::send(char buf[], short size)
{
	int add = 3;
	char *sendBuf;

	for(int i=0; i<size; i++)
	{
		if(buf[i]=='A' || buf[i]=='B')
			add++;
	}

	sendBuf = new char[size+add];
	sendBuf[0]='A';
	sendBuf[size+add-2]='A';
	sendBuf[size+add-1]='\r';

	int a,i;
	a=1;

	for(i=0; i<size; i++)
	{
		if(!(buf[i]=='A' || buf[i]=='B'))
			sendBuf[i+a]=buf[i];

		else if(buf[i]=='A')
	    {
			sendBuf[i+a]='B';
			sendBuf[i+a+1]='C';
			a++;
	    }
		else
		{
			sendBuf[i+a]='B';
			sendBuf[i+a+1]='D';
			a++;
		}
	}
		

	for(int i=0; i<size+add+2; i++)
		std::cout<<sendBuf[i];
		std::cout<<std::endl;

	v24Puts(serialPort,sendBuf);
	delete[] sendBuf;
}


int Link::receive(char buf[], short size)
{
	int bytesRead =0;
	int decr;
	char recieveBuf[size*2];

	char c;
	while(c!='A')
			v24Gets(serialPort,&c,1);

	do
	{
		v24Gets(serialPort,&c,1);
		recieveBuf[bytesRead]=c;
		bytesRead++;
		if(bytesRead > size*2)
		{
			std::cout<<"Error in reading bytes. Size of data sent is too big"<<std::endl;
			return -1;
		}
	}while(c!='A'); //Slut modtaget
			
	int a = 0;
	for(int i=0; i<size; i++)
	{
		if(!(recieveBuf[i+a]=='B' && recieveBuf[i+1+a]=='C') && !(recieveBuf[i+a]=='B' && recieveBuf[i+1+a]=='D') && !(recieveBuf[i+a]=='A'))
			buf[i]=recieveBuf[i+a];
		else if(recieveBuf[i+a]=='B' && recieveBuf[i+1+a]=='C')
		{
			buf[i]='A';
			a++;
		}
	    else if(recieveBuf[i+a]=='B' && recieveBuf[i+1+a]=='D')
	    {
			buf[i]='B';
			a++;
		}
	}

	return bytesRead-a-2;
}

Link::~Link()
{
	if(serialPort != NULL)
		v24ClosePort(serialPort);
	if(buffer != NULL)
		delete [] buffer;
}
