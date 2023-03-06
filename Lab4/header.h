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
#define MAXLINE = 140010
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
void setupAdress(struct sockaddr_in *address, char *host, int port);
int getFile(const char *filename, char *buffer);
int sendFile(int socket, char *filename, int* size);
int readFile(int socket, char *filename, int* size);

int charToInt(char IntIn);
char intToChar(int IntIn);
void encode(char *inBuffer, char *outBuffer, char *key);
void decode(char *inBuffer, char *outBuffer, char *key);

int readSocket(int socket, char *buffer, int size);
int sendSocket(int socket, char *buffer, int *size);






#endif