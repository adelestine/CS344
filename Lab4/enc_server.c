#include "header.h"

int main(int argc, char* argv[]){
    socklen_t sizeOfClientInfo;
    pid_t pid;
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
        perror("ERROR opening socket");
        exit(1);
    }

    // bind to the port and listen
    if (bind(listenSocketFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0){
        perror("ERROR on binding");
        close(listenSocketFD);
        exit(1);
    }
    listen(listenSocketFD, MAX_CLIENTS); // 5 is the max number of connections

    int pidStatus;
    int allowlen = strlen(SERVER_ALLOW_MESSAGE);
    int charsRead;
    while (1){
        // accept a connection, blocking if one is not available until one connects
        sizeOfClientInfo = sizeof(clientAddress); // get the size of the address for the client that will connect
        int establishedConnectionFD = accept(listenSocketFD, (struct sockaddr *)&clientAddress, &sizeOfClientInfo); // accept
        if (establishedConnectionFD < 0){
            perror("ERROR on accept");
            close(listenSocketFD);
            exit(1);
        }

        // fork a new process to handle the connection
        pid = fork();
        if (pid < 0){
            perror("ERROR on fork");
            close(listenSocketFD);
            exit(1);
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
            if(readSocket(establishedConnectionFD, buffer, bufferLen) < 0){
                perror("ERROR reading from socket");
                close(establishedConnectionFD);
                exit(1);
            }
            if (strcmp(buffer, ENC_CONFIRM) != 0){
                int n = strlen(SERVER_BAD_PORT_MESSAGE);
                if (sendSocket(establishedConnectionFD, SERVER_BAD_PORT_MESSAGE, &n) < 0){
                    perror("ERROR writing to socket");
                    close(establishedConnectionFD);
                    exit(1);
                }
                exit(1);
            }
            // checks passed
            //send responce to client
            if(sendSocket(establishedConnectionFD, SERVER_ALLOW_MESSAGE, &allowlen) < 0){
                perror("ERROR writing to socket");
                close(establishedConnectionFD);
                exit(1);
            }
            memset(buffer, 0, MAXLINE);
            charsRead = 0;

            // get file length
            while((charsRead = recv(establishedConnectionFD, buffer, MAXLINE, 0)) == 0);
            bufferLen = atoi(buffer);
            charsRead = 0;


            // proceed to get file
            if(sendSocket(establishedConnectionFD, SERVER_ALLOW_MESSAGE, &allowlen) < 0){
                perror("ERROR writing to socket");
                close(establishedConnectionFD);
                exit(1);
            }
            memset(buffer, 0, MAXLINE);

            // get plaintext and key
            if(readSocket(establishedConnectionFD, buffer, bufferLen) < 0){
                perror("ERROR reading from socket");
                close(establishedConnectionFD);
                exit(1);
            }
            //process
            char* remain;
            char* token = strtok_r(buffer, FILE_DELIM, &remain);
            strcpy(plainMes, token);
            token = strtok_r(NULL, FILE_DELIM, &remain);
            strcpy(key, token);
            encode(plainMes, encodedMes, key );
            int encodedLen = strlen(encodedMes);


            // send encoded message
            int subEncodedLen = strlen(encodedMes);
            if(sendSocket(establishedConnectionFD, encodedMes, &subEncodedLen) < 0){
                perror("ERROR writing to socket");
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