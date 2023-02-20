#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


void clearstdin();

struct Link
{
    char *data;
    struct Link *next;
};

struct inputStr
{
    char *input;
    int length;
    bool isBackground;
    FILE *inputFile;
    FILE *outputFile;
    char *command;
    struct Link *args;
    int argCount;
};


int setInputFile(struct inputStr * inputStr, int * i);
int getUserInput(struct inputStr * inputStr);




#endif // !HEADER_H
