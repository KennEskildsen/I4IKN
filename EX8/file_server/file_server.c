/*
 ============================================================================
 Name        : file_server.c
 Author      : Lars Mortensen
 Version     : 1.0
 Description : file_server in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <lib.h>

void sendFile(char *fileName, long fileSize, int outToClient);

/*
 * Opretter en socket.
 * Venter på en connect fra en klient.
 * Modtager filnavn
 * Finder filstørrelsen
 * Kalder metoden sendFile
 * Lukker socketen og programmet
 *
 * @param argc Antal argumenter
 * @param argv Argumenterne i et array
 */
int main(int argc, char *argv[])
{
	// TO DO Your own code
}

/*
 * Sender filstørrelsen
 * Hvis filstørrelsen = 0 sendes indhold af filen ikke
 * ellers 
 * åbnes filen for at læse den og sende indhold i blokke af 1000 bytes til klienten.
 * Luk filen. 
 *
 * @param fileName Det fulde filnavn incl. evt. stinavn.
 * @param fileSize Størrelsen på filen.
 * @param outToClient Stream for at skrive til serveren.
 */
void sendFile(char *fileName, long fileSize, int outToClient)
{
	// TO DO Your own code
}