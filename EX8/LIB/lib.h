/*
 * lib.h
 *
 *  Created on: Sep 8, 2012
 *      Author: Lars Mortensen
 */

#ifndef LIB_H_
#define LIB_H_

#define PORT 9000
#define BUFSIZE 1000

void error(const char *msg);
const char *extractFileName(const char *fileName);
const char *readTextTCP(char *text, int length, int inFromServer);
void writeTextTCP(int outToServer, char *line);
const long getFileSizeTCP(int inFromServer);
const long check_File_Exists(char *fileName);

#endif /* LIB_H_ */