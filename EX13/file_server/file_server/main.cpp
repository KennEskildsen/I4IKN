#include <iostream>
#include "Transport.h"

#define BUFSIZE 1000

using namespace std;

Transport::Transport *transportlayer;

int main(int argc, char** argv)
{
	transportlayer = new Transport::Transport(BUFSIZE);
	char buf[BUFSIZE];
	transportlayer->receive(buf,BUFSIZE);

	//cout<<"Received: "<<buf<<endl;
	
	return 0;
}