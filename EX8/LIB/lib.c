/*
 * lib.c
 *
 *  Created on: Sep 8, 2012
 *      Author: Lars Mortensen
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/stat.h>
#include <lib.h>

/**
 * Udskriver fejlmeddelelse og stoppe programmet
 *
 * @param msg Fejlmeddelelse
 */
void error(const char *msg)
{
    perror(msg);
    exit(0);
}

/**
 * Udtrækker et filnavn fra en tekststreng
 *
 * @param fileName Filnavn incl. evt. stinavn
 * @return Kun filnavn
 */
const char *extractFileName(const char *fileName)
{
	char *ecn;
   	return ((ecn = strrchr(fileName,'/'))==0 ? fileName : ++ecn);
}

/**
 * Læser en tekststreng fra en socket
 *
 * @param inFromServer stream that holds data from server
 * @return En streng modtaget fra en socket
 */
const char *readTextTCP(char *text, int length, int inFromServer)
{
    char ch;
    int pos=0;

    read(inFromServer, &ch, 1);
    while(ch != 0)
    {
    	if(pos < length)
    		text[pos++] = ch;
        read(inFromServer, &ch, 1);
    }
	return text;
}

/**
 * Skriver en tekststreng til en socket med 0 terminering
 *
 * @param outToServer Stream hvortil der skrives data til socket
 * @param line Teksten der skal skrives til socket
 */
void writeTextTCP(int outToServer, char *line)
{
	write(outToServer, line, strlen(line)+1);
}

/**
 * Konverter en streng som indeholder filstørrelsen in ascii format til en talværdi
 *
 * @param inFromServer Stream som indeholder data fra socket
 * @return Filstørrelsen som talværdi
 */
const long getFileSizeTCP(int inFromServer)
{
	char buffer[256] = {0};
	readTextTCP(buffer, sizeof(buffer), inFromServer);
   	return atol(buffer);
}

/**
 * Se om filen fileName eksisterer
 *
 * @param fileName Filnavn
 * @return Størrelsen på filen, 0 hvis den ikke findes
 */
const long check_File_Exists(char *fileName)
{
	struct stat sts;
	if ((stat (fileName, &sts)) == -1)
		return 0;

	return sts.st_size;
}
