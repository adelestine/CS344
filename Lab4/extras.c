#include "header.h"
// Path: header.h

/*
 * sets up the address struct for the server
 *
 * 
*/
void setupAdress(struct sockaddr_in *servaddr, char *host, int port){
    // set all the bytes in the struct to 0
    bzero(servaddr, sizeof(*servaddr)); // Works the same as memset but is more portable
    // set the address family to AF_INET
    servaddr->sin_family = AF_INET;
    // set the port to the port passed in
    servaddr->sin_port = htons(port);
    //determine host or clinet
    if (strcmp(host, "INADDR_ANY") == 0){
        servaddr->sin_addr.s_addr = INADDR_ANY;
    } else {
        // get DNS entry for host
        struct hostent *hostEntry = gethostbyname(host);
        if (hostEntry == NULL){
            fprintf(stderr, "Error: no such host as %s\n", host);
            exit(1);
        }
        // copy the host address to the server address
        memcpy(&servaddr->sin_addr.s_addr, hostEntry->h_addr_list[0], hostEntry->h_length);
    }
}

/*
 * extracts a files contents into an array one character at a time
 * obtained from https://stackoverflow.com/questions/14002954/c-programming-how-to-read-the-whole-file-contents-into-a-buffer
 * and from https://www.geeksforgeeks.org/c-program-to-count-the-number-of-characters-in-a-file/
*/
int getFile(const char *filename, char *buffer){
    int count = 0;
    FILE *fp = fopen(filename, "r");
    if (fp == NULL){
        fprintf(stderr, "Error: could not open file %s\n", filename);
        return -1;
    }
    char c;
    for (c = getc(fp); c != EOF && c != '\n' ; c = getc(fp)){
        if(!isupper(c) && c != ' '){
            fprintf(stderr, "Error: file %s contains invalid characters\n", filename);
            fclose(fp);
            return -1;
        }
        buffer[count] = c;
        count++;
    }
    buffer[count] = '\0';
    fclose(fp);
    return count;
}

// converts a character to an int NOT ASCII
int charToInt(char c){
    if (c == ' '){
        return 26;
    } else {
        return c - 'A';
    }
}
//converts an int to a character NOT ASCII
char intToChar(int i){
    if (i == 26){
        return ' ';
    } else {
        return i + 'A';
    }
}


//encodes a text entry
void encode(char *inBuffer, char *outBuffer, char *key){
    int i = 0;
    while (inBuffer[i] != '\0'){
        int in = charToInt(inBuffer[i]);
        int keyInt = charToInt(key[i]);
        int out = (in + keyInt) % 27;
        outBuffer[i] = intToChar(out);
        i++;
    }
    outBuffer[i] = '\n';
    outBuffer[i+1] = '\0';
}


//decodes a text entry
void decode(char *inBuffer, char *outBuffer, char *key){
    int i = 0;
    while (inBuffer[i] != '\0'){
        int in = charToInt(inBuffer[i]);
        int keyInt = charToInt(key[i]);
        int out = (in - keyInt) % 27;
        if (out < 0){ // if the result is negative add 27 to make it positive
            out += 27;
        }
        outBuffer[i] = intToChar(out);
        i++;
    }
    // add a newline and null terminator to the end of the string to clean it up
    outBuffer[i] = '\n';
    outBuffer[i+1] = '\0';
}

//reads all data from a socket
int readSocket(int socket, char *buffer, int size){ //possibly not working???
    int total = 0;
    int n = 0;
    char temp[MAXLINE] = {0, };
    //int i = 0;
    //fprintf(stderr, "size: %d\n", size);
    while (total < size){
        //n = read(socket, buffer + total, size - total); // must leave space for the null terminator
        //getting suck in here
        n = recv(socket, temp, MAXLINE - 1, 0);// 
        if (n == -1){break;}
        if(strcmp(temp, SERVER_BAD_PORT_MESSAGE) == 0){break;}
        strcat(buffer, temp);
        if(strcmp(temp, SERVER_ALLOW_MESSAGE) == 0){break;}
        memset(temp, 0 , MAXLINE);
        total += n;

    }
    //fprintf(stderr, "got out\n");
    return n == -1 ? -1 : 0;
}

//sends all data to a socket
int sendSocket(int socket, char *buffer, int *size){
    int total = 0;
    int n = 0;
    int bytesRemining = *size;
    //fprintf(stderr, "size: %d\n", *size);
    while (total < *size){
        n = send(socket, &buffer[total], bytesRemining, 0);
            if (n == -1){
                //fprintf(stderr, "Error: could not send to socket\n");
                break;
            }
        total += n;
    }
    *size = total;
    return n == -1 ? -1 : 0;
}