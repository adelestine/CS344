#ifndef HEADER_H
#define HEADER_H
//include statements
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

//define statements
#define MAX_MESSAGE_SIZE = 140010
#define SERVER_ALLOW_MESSAGE = "connected"
#define SERVER_BAD_PORT_MESSAGE = "bad port"
#define MAX_CLIENTS = 5
#define FILE_DELIM = "/n"


//structs
struct adress{
    char *host;
    char *port;
};

//function prototypes
void setupAdress(struct adress *address, char *host, char *port);
int getFile(char *filename, const char *buffer);
int sendFile(int socket, char *filename, int* size);
int readFile(int socket, char *filename, int* size);

int charToInt(char IntIn);
char intToChar(int IntIn);
void encode(char *buffer, int size);
void decode(char *buffer, int size);






#endif