#include "header.h"

int main(int argc, char* argv[]){
    //announce filename
    //fprintf(stderr, ">>enc_server.c\n"); // for enc_server.c use ">>"
    socklen_t sizeOfClientInfo;
    pid_t pid;
    int establishedConnectionFD;
    struct sockaddr_in serverAddress, clientAddress;

    // check for correct number of arguments
    if (argc < 2){
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(1); // exit with error if not enough arguments
    }

    // set up the address struct for this process (the server)
    setupAdress(&serverAddress, "INADDR_ANY",atoi(argv[1]));

    int listenSocketFD = socket(AF_INET, SOCK_STREAM, 0); // create the socket
    if (listenSocketFD < 0){
        //perror("ERROR opening socket");
        fprintf(stderr, "ERROR opening socket");
        exit(1);
    }

    // bind to the port and listen
    if (bind(listenSocketFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0){
       //perror("ERROR on binding");
        fprintf(stderr, "ERROR on binding");
        close(listenSocketFD);
        exit(1);
    }
    listen(listenSocketFD, MAX_CLIENTS); // 5 is the max number of connections

    int pidStatus;
    int allowlen = strlen(SERVER_ALLOW_MESSAGE);
    int charsRead;

    //fprintf(stderr, ">>waiting for connections...\n");
    while (1){
        // accept a connection, blocking if one is not available until one connects
        sizeOfClientInfo = sizeof(clientAddress); // get the size of the address for the client that will connect
        // int establishedConnectionFD = accept(listenSocketFD, (struct sockaddr *)&clientAddress, &sizeOfClientInfo); // accept
        // if (establishedConnectionFD < 0){
        //     perror("ERROR on accept");
        //     close(listenSocketFD);
        //     exit(1);
        // }


        if((establishedConnectionFD = accept(listenSocketFD, (struct sockaddr *)&clientAddress, &sizeOfClientInfo)) < 0){
            //perror("ERROR on accept");
            fprintf(stderr, "ERROR on accept");
            close(listenSocketFD);
            continue;
        }
        //fprintf(stderr, ">>connection established\n");
        // fork a new process to handle the connection
        pid = fork();
        if (pid < 0){
            //perror("ERROR on fork");
            fprintf(stderr, "error on fork\n");
            close(listenSocketFD);
            continue;
        }
        if (pid == 0){
            // child process
            // setup messages
            char buffer[MAXLINE] = {0, };
            char encodedMes[MAXLINE];
            char plainMes[MAXLINE] = {0, };
            char key[MAXLINE] = {0, };
            //char decodedMes[MAXLINE] = {0, };
            int bufferLen = MAXLINE;

            // respond to connecting client
            //fprintf(stderr, ">>waiting for confirmation\n");
            if(readSocket(establishedConnectionFD, buffer, strlen(ENC_CONFIRM)) < 0){ //getting stuck here
                // perror("ERROR reading from socket");
                fprintf(stderr, "ERROR reading from socket");
                close(establishedConnectionFD);
                exit(1);
            }
            //fprintf(stderr, ">>confirmation received\n");
            if (strcmp(buffer, ENC_CONFIRM) != 0){
                int n = strlen(SERVER_BAD_PORT_MESSAGE);
                if (sendSocket(establishedConnectionFD, SERVER_BAD_PORT_MESSAGE, &n) < 0){
                    //perror("ERROR writing to socket");
                    fprintf(stderr, "ERROR writing to socket");
                    close(establishedConnectionFD);
                    exit(1);
                }
                exit(1);
            }
            // checks passed
            //send responce to client
            if(sendSocket(establishedConnectionFD, SERVER_ALLOW_MESSAGE, &allowlen) < 0){
                //perror("ERROR writing to socket");
                fprintf(stderr, "ERROR writing to socket");
                close(establishedConnectionFD);
                exit(1);
            }
            memset(buffer, 0, MAXLINE);
            charsRead = 0;

           // fprintf(stderr, ">>waiting for file length\n");
            // get file length
            while((charsRead = recv(establishedConnectionFD, buffer, MAXLINE, 0)) == 0);
            bufferLen = atoi(buffer);
            charsRead = 0;
           // fprintf(stderr, ">>file length received: %d\n", bufferLen);

            // proceed to get file
            if(sendSocket(establishedConnectionFD, SERVER_ALLOW_MESSAGE, &allowlen) < 0){
                //perror("ERROR writing to socket");
                fprintf(stderr, "ERROR writing to socket");
                close(establishedConnectionFD);
                exit(1);
            }
            memset(buffer, 0, MAXLINE);

            // get plaintext and key
            if(readSocket(establishedConnectionFD, buffer, bufferLen) < 0){
                //perror("ERROR reading from socket");
                fprintf(stderr, "ERROR reading from socket");
                close(establishedConnectionFD);
                exit(1);
            }
            //returning the wrong message

            //fprintf(stderr, ">>file received: %s\n", buffer);

            //process
            char* remain;
            char* token = strtok_r(buffer, FILE_DELIM, &remain);
            strcpy(plainMes, token);
            token = strtok_r(NULL, FILE_DELIM, &remain);
            strcpy(key, token);
            encode(plainMes, encodedMes, key );
            //fprintf(stderr, ">>encoded message: %s\n", encodedMes);
            int encodedLen = strlen(encodedMes);
           /// fprintf(stderr, ">>encoded message length: %d\n", encodedLen);


            // send encoded message
            int subEncodedLen = encodedLen;
            //fprintf(stderr, ">>encoded message length2: %d\n", subEncodedLen);
            fprintf(stderr, ">>sending encoded message: %s, length %d\n", encodedMes, subEncodedLen);




            
            if(sendSocket(establishedConnectionFD, encodedMes, &subEncodedLen) < 0){ // FIX SEND SOCKET!!!
                //perror("ERROR writing to socket");
                fprintf(stderr, "ERROR writing to socket");
                close(establishedConnectionFD);
                exit(1);
            }

            close(establishedConnectionFD); // close the listening socket
            exit(0);
        }
        else{
            // parent process
            pid_t actPid = waitpid(pid, &pidStatus, WNOHANG);
            
        }
        close(establishedConnectionFD); // close the established connection socket
        sizeOfClientInfo = 0;

    }
    close(listenSocketFD); // close the listening socket
    return 0;
}