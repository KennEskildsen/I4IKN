﻿#ifndef __LINKLAGET_H__
#define __LINKLAGET_H__

#define DELIMITER 'A'

#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

class Link
{
public:
	Link(int bufsize );
	~Link();
	void send(char [], short size);
	int receive(char buf[], short size);
private:
	char *buffer;
};


#endif /* LINKLAGET_H_ */
