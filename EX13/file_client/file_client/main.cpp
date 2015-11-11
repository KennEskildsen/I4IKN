#include <iostream>
#include "Link.h"

#define BUFSIZE 1000

using namespace std;

Link *linklayer;

int main(int argc, char** argv)
{
	linklayer = new Link(BUFSIZE);
	char buf[]="HELLO WORLD!!HELLO WORLD!";
	linklayer->send(buf,25);

	//char buf2[BUFSIZE]={0};
	//linklayer->receive(buf2,BUFSIZE);

	//cout<<"Got the message: "<<buf2<<endl;
	
	return 0;
}
