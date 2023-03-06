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

    
}