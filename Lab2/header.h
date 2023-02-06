#ifndef header_h
#define header_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

void flushStdin(void);
void printMenu(int, char *);

struct file
{
    char * name;
    int size;
    struct file * next;
};

struct movie
{
    char *          Title;
    char *          Lang[5];
    int             numberOfLangs;
    double          Rating;
    int             Year;
    struct movie *  next;
};



void findLargestFile(char *, struct file *);
void processFile(struct file *);
struct file * readDirectory(char * );

//from lab1
void cutLangs(struct movie * currMovie, char * token);
struct movie *createMovie(char *currLine);
struct movie *processMovies(char *filePath, int *movieNum);





#endif