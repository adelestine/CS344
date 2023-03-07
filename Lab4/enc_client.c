#include "header.h"

int main(int argc, char *argv[])
{
    int sockfd, n;// n is the number of bytes read, sockfd is the socket file descriptor
    char recvline[MAXLINE + 1];
    struct sockaddr_in servaddr;
    // check for correct number of arguments
    if (argc < 3){
        fprintf(stderr, "usage: %s <IPaddress> <port>", argv[0]);
        exit(1); // exit with error if not enough arguments
    }

    //create a socket and check for errors
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        fprintf(stderr, "socket error");
        exit(1);
    }

    bzero(&servaddr, sizeof(servaddr)); // this is stupid and I love it, it sets all the bytes in the struct to 0

    //setupAdress(&servaddr, argv[1], argv[3]); // set up the address struct where argv[1] is the host and argv[3] is the port
    setupAdress(&servaddr, "localhost", atoi(argv[3])); // host will always be localhost for this assignment

    // set the socket options to reuse the address
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int)); //sets the socket options to reuse the address

    // connect to the server and check for errors
    if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0){
        fprintf(stderr, "connect error");
        close(sockfd);
        exit(1);
    }

    //store contents of file into internal buffer
    char plainText[MAXLINE] = {0, };
    char cipherText[MAXLINE] = {0, };
    int ptchars = getFile(argv[2], plainText); // get the number of characters in the file and load
    int ctchars = getFile(argv[2], cipherText); // get the number of characters in the file and load

    // check getfile for errors
    if (ptchars < 0){
        fprintf(stderr, "error reading file");
        close(sockfd);
        exit(1);
    }
    if (ctchars < 0){
        fprintf(stderr, "error reading file");
        close(sockfd);
        exit(1);
    }
    // make sure the plaintext is smaller than the cipher text
    if (ptchars > ctchars){
        fprintf(stderr, "error: cipher text is smaller than plain text");
        close(sockfd);
        exit(1);
    }

    // send the plaintext to the server
    int confimlen = strlen(ENC_CONFIRM);
    if (sendSocket(sockfd, ENC_CONFIRM, &confimlen) < 0){
        fprintf(stderr, "error sending confirmation");
        close(sockfd);
        exit(1);
    }
    int filelen = ptchars +ctchars +2;
    char * buffer = calloc(filelen, sizeof(char));
    if (buffer == NULL){
        fprintf(stderr, "error allocating memory");
        close(sockfd);
        exit(1);
    }
    int readlen = strlen(SERVER_ALLOW_MESSAGE);
    if (readSocket(sockfd, buffer, readlen) < 0){
        fprintf(stderr, "error reading from socket");
        close(sockfd);
        exit(1);
    }
    if (strcmp(buffer, SERVER_BAD_PORT_MESSAGE) == 0 || strcmp(buffer, SERVER_ALLOW_MESSAGE) != 0){
        fprintf(stderr, "error: server did not allow connection on port %s", argv[3]);
        close(sockfd);
        exit(2);
    }

    //send the file length
    memset(buffer, 0, strlen(buffer));
    sprintf(buffer, "%d", filelen);
    if (sendSocket(sockfd, buffer, &filelen) < 0){
        fprintf(stderr, "error sending file length");
        close(sockfd);
        free(buffer);
        exit(1);
    }

    if(strcmp(buffer, SERVER_BAD_PORT_MESSAGE) == 0 || strcmp(buffer, SERVER_ALLOW_MESSAGE) != 0){
        fprintf(stderr, "error: server did not allow connection on port %s", argv[3]);
        close(sockfd);
        free(buffer);
        exit(2);
    }

    //send the file contents

    memset( buffer, 0, strlen(buffer));
    strcat(buffer, plainText);
    strcat(buffer, FILE_DELIM);
    strcat(buffer, cipherText);
    strcat(buffer, FILE_DELIM);
    if (sendSocket(sockfd, buffer, &filelen) < 0){
        fprintf(stderr, "error sending file contents");
        close(sockfd);
        free(buffer);
        exit(1);
    }

    // read the response from the server
    readlen = ptchars + 1;
    char * response = calloc(readlen +1 , sizeof(char));
    if (response == NULL){
        fprintf(stderr, "error allocating memory");
        close(sockfd);
        free(buffer);
        exit(1);
    }
    if (readSocket(sockfd, response, readlen) < 0){
        fprintf(stderr, "error reading from socket");
        close(sockfd);
        free(buffer);
        free(response);
        exit(1);
    }
    printf(response);
    free(buffer);
    free(response);
    close(sockfd);
    return 0;
}