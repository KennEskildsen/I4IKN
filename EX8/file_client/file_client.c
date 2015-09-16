/*
 ============================================================================
 Name        : file_client.c
 Author      : Lars Mortensen
 Version     : 1.0
 Description : file_client in C, Ansi-style
 ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <lib.h>

void receiveFile(char *fileName, int socketfd);

/*
 * Opretter en socket.
 * Opretter en forbindelse til en server.
 * Sender filnavn
 * Kalder metoden receiveile
 * Lukker socketen og programmet
 *
 * @param argc Antal argumenter
 * @param argv Argumenterne i et array
 */
int main(int argc, char *argv[])
{
	// TO DO Your own code
}

/**
 * Modtager filstørrelsen og udskriver meddelelsen: "Filen findes ikke" hvis størrelsen = 0
 * ellers
 * åbnes filen for at skrive de bytes som senere modtages fra serveren (HUSK kun selve filnavn)
 * Modtag og gem filen i blokke af 1000 bytes indtil alle bytes er modtaget.
 * Luk filen, samt input output streams
 *
 * @param fileName Det fulde filnavn incl. evt. stinavn
 * @param sockfd Stream for at skrive til/læse fra serveren
 */
void receiveFile(char *fileName, int sockfd)
{
	// TO DO Your own code
}