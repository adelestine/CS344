#include "header.h"
//imported from other Lab1 spaces
void printMenu(int menuNum, char * output){
    switch (menuNum)
    {
    case 1:
        printf(  "1. Select file to process\n");
        printf(  "2. Exit from the program\n \n");

        printf("Enter a choice from 1 or 2: ");
        scanf("%s", output);
        flushStdin();

        break;
    case 2:
        printf( "Which file do you want to process?\n");
        printf( "Enter 1 to pick the largest file \n");
        printf( "Enter 2 to pick the smallest file \n");
        printf( "Enter 3 to pick specify the name of a file\n");
        printf("Enter a choice from 1 to 3: ");

        //flushStdin();
        scanf("%s", output);
        break;
    default:
        break;
    }
    

}
void flushStdin(void) {
	int ch;
	while (((ch = getchar()) != '\n') && (ch != EOF));
}
void findLargestFile(char * output, struct file * head){
    struct file * current = head;
    struct file * largest = NULL;
    int first = 0;
    //read the file names and sizes into a linked list
    while (current != NULL)
    {
        if (strstr(current->name, ".csv") != NULL && strstr(current->name, "movies_") != NULL)
        {
            if(first == 0){
                largest = current;
                first = 1;
            }else if(current->size > largest->size){
                largest = current;
            }
        }
        current = current->next;
    }
    processFile(largest);
    //traverse the linked list and find the largest file
    //find the largest file in the directory
    //save the file name in output
    strcpy(output, largest->name);
    //output = largest->name;
}
void freeMovies(struct movie *head){ // found on Stack Overflow
    struct movie* tmp; 
    while (head != NULL)
    {
        tmp = head;
        head = head->next;
        
        //free(Mlist->Rating);
        free(tmp->Title);
        for (int i = 0; i < tmp->numberOfLangs; i++)
        {
            free(tmp->Lang[i]);
        }
        free(tmp);
        //free(Mlist->Year);
        //free(Mlist->next);
    }
}
struct movie *createMovie(char *currLine)
{
    struct movie *currMovie = malloc(sizeof(struct movie));
    currMovie->numberOfLangs = 0;
    char *saveptr;
    char *token;
    char *endToken;


    // The first token is the Title
    token = strtok_r(currLine, ",", &saveptr);
    currMovie->Title = calloc(strlen(token) + 1, sizeof(char)); //+1 for null terminator
    strcpy(currMovie->Title, token);

    // The next token is the Year
    token = strtok_r(NULL, ",", &saveptr);
    currMovie->Year = atoi(token);
    //save token for all Langs
    token = strtok_r(NULL, ",", &saveptr);
    cutLangs(currMovie, token);


    //token = strtok(currLine, "]")
    // The last token is the Rating
    token = strtok_r(NULL, "\n", &saveptr);
    //currMovie->Rating = calloc(8, sizeof(double));
    currMovie->Rating = strtod(token, &endToken);
    //strcpy(currMovie->Rating, token);



    // Set the next node to NULL in the newly created movie entry
    currMovie->next = NULL;
    return currMovie;
}
struct movie *processMovies(char *filePath, int *movieNum)
{
    // Open the specified file for reading only
    FILE *movieFile = fopen(filePath, "r");

    char *currLine = NULL;
    size_t len = 0;
    size_t nread = getline(&currLine, &len, movieFile); //lol makes skip first line
    char *token;
        int temp = 0;
        temp = *movieNum;
    // The head of the linked list
    struct movie *head = NULL;
    // The tail of the linked list
    struct movie *tail = NULL;

    // Read the file line by line
    while ((nread = getline(&currLine, &len, movieFile)) != -1)
    {
        // Get a new movie node corresponding to the current line
        struct movie *newNode = createMovie(currLine);
        temp++;

        // Is this the first node in the linked list?
        if (head == NULL)
        {
            // This is the first node in the linked link
            // Set the head and the tail to this node
            head = newNode;
            tail = newNode;
        }
        else
        {
            // This is not the first node.
            // Add this node to the list and advance the tail
            tail->next = newNode;
            tail = newNode;
        }
        
    }
    free(currLine);
    fclose(movieFile);
    *movieNum = temp;
    return head;
}
// Chops the languages up into an array format parseable to the program
void cutLangs(struct movie * currMovie, char * token){
    int tokenLenght;
    int numberOfLangs = 0;
    char * savePtr;
    char * tokOftok = strtok_r(token, ";", &savePtr);
    while (tokOftok != NULL && numberOfLangs < 5)
    {
        tokenLenght = strlen(tokOftok) -1;
        if(tokOftok[0] == '[' && tokOftok[tokenLenght] == ']'){
            //only 1 entry
            //remove ends
            
            tokOftok[tokenLenght] = '\0';
            for (int i = 0; i < tokenLenght-1; i++)
            {
                tokOftok[i] = tokOftok[i+1];
            }
            tokenLenght -= 1;
            tokOftok[tokenLenght] = '\0';
            
        }else if(tokOftok[0] == '['){
            for (int i = 0; i < tokenLenght; i++)
            {
                tokOftok[i] = tokOftok[i+1];
                //printf("%c", tokOftok[i]);
            }
            tokOftok[tokenLenght] = '\0';
            //printf("Length of tokoftok: %d for Token: %s\n", tokenLenght, tokOftok);

        }else if(tokOftok[tokenLenght] == ']'){
            tokOftok[tokenLenght] = '\0';
            tokenLenght -= 1;
        }
        currMovie->Lang[numberOfLangs] = calloc(tokenLenght+1, sizeof(currMovie->Lang[numberOfLangs]));
        strcpy(currMovie->Lang[numberOfLangs], tokOftok);
        numberOfLangs +=1;
        tokOftok = strtok_r(NULL, ";", &savePtr);
        //*currMovie->Lang[numberOfLangs] = *tokOftok;
    }
    currMovie->numberOfLangs = numberOfLangs;
}
void processFile(struct file * file){
    //process the file
    printf("Now processing the chosen file named %s\n", file->name);
    FILE * fp = fopen(file->name, "r");     //open the file
    //read the file
    //process the file  
    int movieNum = 0;
    struct movie * top = processMovies(file->name, &movieNum);
    struct movie * currMovie = top;
  
    //create directory titled delestic.movies.randomNumber
    int dirNum = random() % 100000; //random number between 0 and 99999
    char * dirName = calloc(30, sizeof(char)); //TODO: FIX MEM LEAK
    sprintf(dirName, "delestic.movies.%d", dirNum);    //premessions of the file must be set to rwxr-x---
    // ie the owner has read, write, and execute permissions, the group has read and execute permissions, and others have no permissions
    #ifdef __linux__                        
    mkdir(dirName, 0750); // rwxr-x---
    #elif _WIN32
    mkdir(dirName);
    #endif


    //In this file create a new file for each year a movie was released.
    //permissions of the file must be set to rw-r----- 640
    int years[movieNum];

    char * fileName = calloc(30, sizeof(char));
    //fileName = "null :/";
    
    sprintf(fileName, "%s/%d.txt", dirName, currMovie->Year);  //funky stuff  
    FILE * yearFile = fopen(fileName, "w");
    fprintf(yearFile, "%s\n", currMovie->Title);
    fclose(yearFile);
    chmod(fileName, 0640);
    free(fileName);
    years[0] = currMovie->Year;
    currMovie = currMovie->next;
    int yearCount = 1;
    while (currMovie != NULL)
    {
        //create a file for each year
        int yearExists = 0;
        for (int i = 0; i < yearCount; i++)
        {
            if (years[i] == currMovie->Year)
            {
                yearExists = 1;
            }
        }
        if (yearExists == 0)
        {
            //create a new file
            char * fileName = calloc(30, sizeof(char));
            sprintf(fileName, "%s/%d.txt", dirName, currMovie->Year);   
            FILE * yearFile = fopen(fileName, "w");
            fprintf(yearFile, "%s\n", currMovie->Title);
            fclose(yearFile);
            chmod(fileName, 0640);
            free(fileName);
            years[yearCount] = currMovie->Year;
            yearCount += 1;
        }else{
            //append to the file
            char * fileName = calloc(30, sizeof(char));
            sprintf(fileName, "%s/%d.txt", dirName, currMovie->Year);   
            FILE * yearFile = fopen(fileName, "a");
            fprintf(yearFile, "%s\n", currMovie->Title);
            fclose(yearFile);
            free(fileName);
        }
        currMovie = currMovie->next;
    }

    //ie the owner has read and write permissions, the group has read permissions, and others have no permissions
    //the file must be named year.txt ie 1999.txt

    //for each movie that is released in that year create a new line with that movies title.


    //close the file
    free(dirName);
    fclose(fp);
    freeMovies(top);

}
struct file * loadFileList(DIR * cwd){
    //read the file names and sizes into a linked list
    //return the head of the linked list
    struct dirent * entry;
    struct stat statbuf;
    struct file * head = NULL; //head of the linked list
    while (entry=readdir(cwd)) //read the directory
    {
        if (entry->d_type == DT_REG /*&& (strstr(entry->d_name, ".csv") != NULL) && (strstr(entry->d_name, "movies_") != NULL)*/) //if the entry is a regular file
        {  
            struct file * newFile = malloc(sizeof(struct file)); //create a new file 
            newFile->name = entry->d_name;                     //set the name
            stat(newFile->name, &statbuf);                     //get the size
            newFile->size = statbuf.st_size;                    //set the size
            newFile->next = NULL;                               //set the next to null
            if (head == NULL)
            {
                head = newFile;                                 //set the head to the new file if it is the first file
            }
            else
            {
                struct file * current = head;                   //set the current to the head
                while (current->next != NULL)                   //traverse the list until the end
                {
                    current = current->next;                    //set the current to the next
                }
                current->next = newFile;                        //set the next of the current to the new file
            }
            
        }
        
    }
    return head;

}
void findSmallestFile(char * output, struct file * head){
    //find the smallest file and return the name
    struct file * current = head;
    struct file * smallest = NULL;
    int first = 0;
    while (current != NULL)
    {
        if((strstr(current->name, ".csv") != NULL) && (strstr(current->name, "movies_") != NULL)){
            printf("%s\n", current->name);
            if (first == 0)
            {
                smallest = current;
                first = 1;
            }else if (current->size < smallest->size)
            {
                smallest = current;
            }
       }
        current = current->next;
    }
    printf("Smallest File: %s\n", smallest->name);
    processFile(smallest);
    strcpy(output, smallest->name);
}
/*
Find the file with the name and return the name

*/
void findFileWithName(char * output, struct file * head, char * name){
    //find the file with the name and return the name
    struct file * current = head;
    while (current != NULL)
    {
        if (strcmp(current->name, name) == 0)
        {
            strcpy(output, current->name);
            processFile(current);
            break;
        }
        current = current->next;
    }
    strcpy(output, "File Not Found");
    
}
/*
free all the memory allocated for the linked list
specificaly the memory allocated for the file names
return nothing
*/
void freeall(struct file * head){
    struct file * current = head;
    while (current != NULL)
    {
        struct file * temp = current;
        current = current->next;
        free(temp);
    }
    free(current);
    
}
int main(int argc, char *argv[])
{
    char usrIn[20 + 1];
    int exit = 0;
    char name[256];
    DIR *cwd; //current working directory
    //OBTAINED FROM https://c-for-dummies.com/blog/?p=3246
    cwd = opendir("."); //open current working directory
    if (cwd == NULL) //if current working directory is null
    {
        printf("Error: Cannot open current working directory\n");
        return EXIT_FAILURE;
    }
    struct file * head = loadFileList(cwd); //load the file list //PRESERVE!!!


    while (exit == 0) //main loop for selector
    {
        printMenu(1, usrIn); // print main menu and save in usrIn
        if (strcmp(usrIn, "1") == 0) //if user selects 1
        {
            printMenu(2, usrIn); //print file selector menu and save in usrIn
            if (strcmp(usrIn, "1") == 0) //if user selects 1
            {
                //pick the largest file
                findLargestFile(name, head);
            }
            else if (strcmp(usrIn, "2") == 0) //if user selects 2
            {
                //pick the smallest file
                findSmallestFile(name, head);
            }
            else if (strcmp(usrIn, "3") == 0) //if user selects 3
            {
                //pick a file by name
                printf("Please enter the name of the file you would like to process: ");
                scanf("%s", usrIn);
                findFileWithName(name, head, usrIn);
                if (strcmp(name, "File Not Found") == 0)
                {
                    printf("File Not Found\n");
                }
            }
            else
            {
                printf("Invalid input, please try again\n");
            }
            
        }
        else if (strcmp(usrIn, "2") == 0) //if user selects 2
        {
            exit = 1;
        }
        else
        {
            printf("Invalid input, please try again\n");
        }
        
    }
    freeall(head);
    closedir(cwd);
    return EXIT_SUCCESS;
    //free(usrIn);
}