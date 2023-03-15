#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <pthread.h>


#define LINE_SEP '\n'
#define PLUS_SIGN "++"
#define SIZE 80
#define CHAR_OUT '^'

struct eLetter
{
    char data[1000];
    int size;
};






#endif