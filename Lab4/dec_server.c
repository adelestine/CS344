#include "header.h"

int main(int argc, char const *argv[])
{
    //announce filename
    //fprintf(stderr, "++dec_server.c\n"); // for dec_server.c use "++"
    struct sockaddr_in serverAddress, clientAddress;
    pid_t pid;
    int listenSocketFD, establishedConnectionFD, portNumber, charsRead, pidStatus;
    int confirmSize = strlen(SERVER_ALLOW_MESSAGE);
    int clientConfirmSize = strlen(DEC_CONFIRM);
    //check for correct number of arguments
    if(argc < 2){
        fprintf(stderr, "Usage: %s  port"  , argv[0]);
        exit(1);
    }
    setupAdress(&serverAddress, "INADDR_ANY", atoi(argv[1]));
    listenSocketFD = socket(AF_INET, SOCK_STREAM, 0);

    if(listenSocketFD < 0){
        fprintf(stderr, "Error: could not open socket");
        exit(1);
    }

    if(bind(listenSocketFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0){
        fprintf(stderr, "Error: could not bind socket");
        close(listenSocketFD);
        exit(1);
    }
    listen(listenSocketFD, MAX_CLIENTS);


    int cliSize = sizeof(clientAddress);
    while(1){

        establishedConnectionFD = accept(listenSocketFD, (struct sockaddr*)&clientAddress, &cliSize);

        if(establishedConnectionFD < 0){
            fprintf(stderr, "Error: could not accept connection");
            close(listenSocketFD);
            //exit(1);
            continue;
        }
        pid = fork();
        if(pid == -1){
            fprintf(stderr, "Error: could not fork");
            close(listenSocketFD);
            close(establishedConnectionFD);
            continue;
        } else if(pid == 0){
            //child process
            //setup chars
            char buffer[MAXLINE] = {0, };
            char key[MAXLINE] = {0, };
            char encodedText[MAXLINE] = {0, };
            char decodedText[MAXLINE];
            int bufferLen = 0;
            printf("S: read from client\n");
            fflush(stdout);
            if(readSocket(establishedConnectionFD, buffer, clientConfirmSize) == -1){
                fprintf(stderr, "Error: could not read from server");
                close(establishedConnectionFD);
                exit(1);
            }
            printf("S: reccived 'dec confirm' from client\n");
            fflush(stdout);
            if(strcmp(buffer, DEC_CONFIRM) != 0){
                fprintf(stderr, "Error: could not read from server");
                close(establishedConnectionFD);
                exit(1);
            }
            printf("S: sending 'confirm' to client\n");
            fflush(stdout);
            if(sendSocket(establishedConnectionFD, SERVER_ALLOW_MESSAGE, &confirmSize) == -1){
                fprintf(stderr, "Error: could not send confirmation to server");
                close(establishedConnectionFD);
                exit(1);
            }
            
            printf("S: sent 'confirm' to client\n");
            fflush(stdout);

            memset(buffer, 0, MAXLINE);
            charsRead = 0;
            printf("S: reading file length from client\n");
            fflush(stdout);
            while((charsRead = recv(establishedConnectionFD, buffer, MAXLINE, 0)) == 0);
            bufferLen = atoi(buffer);
            charsRead = 0;
            printf("S: reccived file length from client\n");
            fflush(stdout);

            printf("S: sending 'confirm' to client #2\n");
            fflush(stdout);
            if(sendSocket(establishedConnectionFD, SERVER_ALLOW_MESSAGE, &confirmSize) == -1){
                fprintf(stderr, "Error: could not send confirmation to server");
                close(establishedConnectionFD);
                exit(1);
            }
            printf("S: sent 'confirm' to client #2\n");
            fflush(stdout);
            printf("S: reading encoded text from client\n");
            fflush(stdout);
            if(readSocket(establishedConnectionFD, buffer, bufferLen) == -1){
                fprintf(stderr, "Error: could not read from server");
                close(establishedConnectionFD);
                exit(1);
            }
            printf("S: reccived encoded text from client\n");
            fflush(stdout);




            char * remain;
            char * token = strtok_r(buffer, FILE_DELIM, &remain);
            strcpy(encodedText, token);
            token = strtok_r(NULL, FILE_DELIM, &remain);
            strcpy(key, token);
            //printf("encodedText: %s key: %s  buffer: %s bufferLen: %d", encodedText, key, buffer, bufferLen);]

            decode(encodedText, decodedText, key);
            int decodedTextLen = strlen(decodedText);
            int decodedTextLen2 = strlen(decodedText);
            printf("S: sending decoded text to client\n");
            fflush(stdout);
            if(sendSocket(establishedConnectionFD, decodedText, &decodedTextLen2) == -1){
                fprintf(stderr, "Error: could not send back to client");
                close(establishedConnectionFD);
                exit(1);
            }
            printf("S: sent decoded text to client\n");
            fflush(stdout);
            close(establishedConnectionFD);
            exit(0);
        }
        else{
            //parent process
            close(establishedConnectionFD);
            pid_t Apid = waitpid(-1, &pidStatus, WNOHANG);
        }
        close(establishedConnectionFD);
    }
    close(listenSocketFD);
    return 0;
}
