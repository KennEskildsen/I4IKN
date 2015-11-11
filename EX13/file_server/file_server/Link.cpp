#include "Link.h"


Link::Link(int bufsize)
{

}


void Link::send(char buf[], short size)
{
	int add = 2;
	char *sendBuf;

	for(int i=0; i<size; i++)
	{
		if(buf[i]=='A' || buf[i]=='B')
			add++;
	}

	sendBuf = new char[size+add];
	sendBuf[0]='A';
	sendBuf[size+add-1]='A';

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

	int fd = open ("/dev/ttyS1", O_RDWR | O_NOCTTY | O_SYNC);
	if (fd < 0)
	{	
        	std::cout<<"Error opening serialport /dev/ttyS1"<<std::endl;
        	return;
	}

	write (fd, sendBuf, size+add+2); 
	close(fd);
	delete[] sendBuf;
}


int Link::receive(char buf[], short size)
{
	int bytesRead =0;
	int decr;
	char recieveBuf[size*2];
	int fd = open ("/dev/ttyS1", O_RDWR | O_NOCTTY | O_SYNC);
	if (fd < 0)
	{	
        	std::cout<<"Error opening serialport /dev/ttyS1"<<std::endl;
        	return -1;
	}

	char c;
	while(c!='A')
			read (fd, &c, 1); //Start modtaget

	do
	{
		read (fd, &c, 1); 
		recieveBuf[bytesRead]=c;
		bytesRead++;
		if(bytesRead > size*2)
		{
			std::cout<<"Error in reading bytes. Size of data sent is too big"<<std::endl;
			return -1;
		}
	}while(c!='A'); //Slut modtaget
			
	int a = 1;
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

	close(fd);
	return bytesRead-a-2;
}

Link::~Link()
{

}
