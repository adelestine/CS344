#include "header.h"

int main(int argc, char *argv[]){
    //announce filename
    fprintf(stderr, "--dec_client.c"); // for dec_client.c use "--"
    //check for correct number of arguments
    if(argc < 3){
        fprintf(stderr, "Usage: %s plaintext key port"  , argv[0]);
        exit(1);
    }

    //set up variables
    int socketFD, portNumber, charsWritten, charsRead;
    struct sockaddr_in serverAddress;
    char * buffer;
    char key[MAXLINE];
    char encodedText[MAXLINE];

    //connect to server
    socketFD = socket(AF_INET, SOCK_STREAM, 0);
    if(socketFD < 0){
        fprintf(stderr, "Error: could not open socket");
        exit(1);
    }
    setupAdress(&serverAddress, "localhost", atoi(argv[3]));
    setsockopt(socketFD, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));

    //actually connect to server
    if(connect(socketFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0){
        fprintf(stderr, "Error: could not connect to server");
        close(socketFD);
        exit(1);
    }

    //check for valid characters in key and plaintext
    int encodedChars = getFile(argv[1], encodedText);
    int keyChars = getFile(argv[2], key);
    if(keyChars == -1 || encodedChars == -1){
        fprintf(stderr, "Error: invalid characters in file");
        close(socketFD);
        exit(1);
    }
    if(keyChars < encodedChars){
        fprintf(stderr, "Error: key is too short");
        close(socketFD);
        exit(1);
    }

    //send server confirmation
    int confirmSize = strlen(DEC_CONFIRM);
    if(sendSocket(socketFD, DEC_CONFIRM, &confirmSize) == -1){
        fprintf(stderr, "Error: could not send confirmation to server");
        close(socketFD);
        exit(1);
    }
    int grandlen = keyChars + encodedChars +2;
    buffer = calloc(grandlen, sizeof(char));
    if(readSocket(socketFD, buffer, grandlen) == -1){
        fprintf(stderr, "Error: could not read from server");
        close(socketFD);
        free(buffer);
        exit(1);
    }
    if(strcmp(buffer, SERVER_ALLOW_MESSAGE) != 0 || strcmp(buffer, SERVER_BAD_PORT_MESSAGE) == 0){
        fprintf(stderr, "Error: could not connect to server");
        close(socketFD);
        free(buffer);
        exit(2);
    }

    //send file length
    memset(buffer, 0, strlen(buffer));
    sprintf(buffer, "%d", grandlen);
    if(sendSocket(socketFD, buffer, &grandlen) == -1){
        fprintf(stderr, "Error: could not send file length to server");
        close(socketFD);
        free(buffer);
        exit(1);
    }
    memset(buffer, 0, strlen(buffer));
    if (readSocket(socketFD, buffer, grandlen) == -1){
        fprintf(stderr, "Error: could not read from server");
        close(socketFD);
        free(buffer);
        exit(1);
    }
    if(strcmp(buffer, SERVER_ALLOW_MESSAGE) != 0 || strcmp(buffer, SERVER_BAD_PORT_MESSAGE) == 0){
        fprintf(stderr, "Error: could not connect to server");
        close(socketFD);
        free(buffer);
        exit(2);
    }

    //send encoded text
    memset(buffer, 0, strlen(buffer));
    strcat(buffer, encodedText);
    strcat(buffer, FILE_DELIM);
    strcat(buffer, key);
    strcat(buffer, FILE_DELIM);
    if(sendSocket(socketFD, buffer, &grandlen) == -1){
        fprintf(stderr, "Error: could not send encoded text to server");
        close(socketFD);
        free(buffer);
        exit(1);
    }

    grandlen = keyChars + 1;
    char * decodedText = calloc(grandlen, sizeof(char));
    if(readSocket(socketFD, decodedText, grandlen) == -1){
        fprintf(stderr, "Error: could not read from server");
        close(socketFD);
        free(buffer);
        free(decodedText);
        exit(1);
    }
    printf(decodedText);
    free(buffer);
    free(decodedText);
    close(socketFD);
    return 0;
}