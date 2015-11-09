#include "Link.h"


Link::Link(int bufsize)
{

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
	int bytesRead;
	int decr;
	char receiveBuf[size];
	int fd = open ("/dev/ttyS1", O_RDWR | O_NOCTTY | O_SYNC);
	if (fd < 0)
	{	
        	std::cout<<"Error opening serialport /dev/ttyS1"<<std::endl;
        	return -1;
	}

	bytesRead = read (fd, receiveBuf, size); 

	if(!(receiveBuf[0]=='A' && receiveBuf[bytesRead-2]=='A'))
	{
		    std::cout<<"Did not receive the full package"<<std::endl;
        	return -1;
	}
			
	int a = 1;
	for(int i=0; i<size; i++)
	{
		if(!(receiveBuf[i+a]=='B' && receiveBuf[i+1+a]=='C') && !(receiveBuf[i+a]=='B' && receiveBuf[i+1+a]=='D') && !(receiveBuf[i+a]=='A'))
			buf[i]=receiveBuf[i+a];
		else if(receiveBuf[i+a]=='B' && receiveBuf[i+1+a]=='C')
		{
			buf[i]='A';
			a++;
		}
	    else if(receiveBuf[i+a]=='B' && receiveBuf[i+1+a]=='D')
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
