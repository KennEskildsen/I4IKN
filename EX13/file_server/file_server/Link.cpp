#include "Link.h"

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
	int add = 2;

	for(int i=0; i<size; i++)
	{
		if(buf[i]=='A' || buf[i]=='B')
			add++;
	}
			
	buffer[0]='A';
	buffer[size+add-1]='A';
	//sendBuf[size+add-1]='\r';

	int a,i;
	a=1;

	for(i=0; i<size; i++)
	{
		if(!(buf[i]=='A' || buf[i]=='B'))
			buffer[i+a]=buf[i];

		else if(buf[i]=='A')
	    {
			buffer[i+a]='B';
			buffer[i+a+1]='C';
			a++;
	    }
		else
		{
			buffer[i+a]='B';
			buffer[i+a+1]='D';
			a++;
		}
	}
		
	std::cout<<"Linklayer has sent: ";
	for(int i=0; i<size+add; i++)
		std::cout<<buffer[i];
		std::cout<<std::endl;

	v24Write(serialPort,(const unsigned char*)buffer,size+add);
}


int Link::receive(char buf[], short size)
{
	int bytesRead =0;
	int decr;

	char c;
	while(c!='A')
			v24Read(serialPort,(unsigned char*)&c,1);

	do
	{
		v24Read(serialPort,(unsigned char*)&c,1);
		buffer[bytesRead]=c;
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
		if(!(buffer[i+a]=='B' && buffer[i+1+a]=='C') && !(buffer[i+a]=='B' && buffer[i+1+a]=='D') && !(buffer[i+a]=='A'))
			buf[i]=buffer[i+a];
		else if(buffer[i+a]=='B' && buffer[i+1+a]=='C')
		{
			buf[i]='A';
			a++;
		}
	    else if(buffer[i+a]=='B' && buffer[i+1+a]=='D')
	    {
			buf[i]='B';
			a++;
		}
	}
		
		std::cout<<"Linklayer received: ";
		for(int i=0; i<bytesRead-a-1; i++)
		std::cout<<buffer[i];
		std::cout<<"\nBytes read: "<<bytesRead-a-1<<std::endl;

	return bytesRead-a-1;
}

Link::~Link()
{
	if(serialPort != NULL)
		v24ClosePort(serialPort);
	if(buffer != NULL)
		delete [] buffer;
}
