// If you are not compiling with the gcc option --std=gnu99, then
// uncomment the following line or you might get a compiler warning
//#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"


//imported from other spaces
//Flushed input buffer
void flushStdin(void) {
	int ch;
	while (((ch = getchar()) != '\n') && (ch != EOF));
}

/* struct for movie information */
struct movie
{
    char *          Title;
    char *          Lang[5];
    int             numberOfLangs;
    double          Rating;
    int             Year;
    struct movie *  next;
};
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




/* Parse the current line which is comma delimited and create a
*  movie struct with the data in each line
*/
struct movie *createMovie(char *currLine)
{
    struct movie *currMovie = malloc(sizeof(struct movie));
    currMovie->numberOfLangs = 0;
    char *saveptr;
    char *token;
    char *endToken;


    // The first token is the Title
    token = strtok_r(currLine, ",", &saveptr);
    currMovie->Title = calloc(strlen(token) + 1, sizeof(char));
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

/*
* Return a linked list of movies by parsing data from
* each line of the specified file.
*/
struct movie *processFile(char *filePath, int *i) // 64 bytes lost
{
    // Open the specified file for reading only
    FILE *movieFile = fopen(filePath, "r");

    char *currLine = NULL;
    size_t len = 0;
    size_t nread = getline(&currLine, &len, movieFile); //lol makes skip first line
    char *token;
        int temp = 0;
        temp = *i;
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
    *i = temp;
    return head;
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

    //free(Mlist);
    //free(savedNext);
    
}
int printMenu(){
    int output;
    printf("1. Show movies released in the specified Year\n");
    printf( "2. Show highest rated movie for each Year\n");
    printf( "3. Show the Title and Year of release of all movies in a specific language\n");
    printf(  "4. Exit from the program\n \n");

    printf("Enter a choice from 1 to 4: ");
    scanf("%d", &output);
    return output;


}

void getYear(int YearGiven, struct movie *head){

    int numMovies = 0;
    while (head != NULL)
    {
        if(head->Year == YearGiven){
            printf("%s \n", head->Title);
            numMovies++;
        }
        head = head->next;
        
    }
    if(numMovies == 0){
        printf("No data about movies released in the Year %d\n \n", YearGiven);
    }
    
}
void lang(struct movie * head){
    printf("Enter the language for which you want to see movies: ");
    char usrIn[21];
    flushStdin();
    scanf("%s", usrIn);
    printf("%s\n" , usrIn);
    while (head != NULL)
    {
        // printf("%s : ", head->Title);
        // for (int i = 0; i < head->numberOfLangs; i++)
        // {
        //     printf("%s", head->Lang[i]);
        // }
        // printf("\n");
        for (int i = 0; i < head->numberOfLangs; i++)
        {
            //printf("%s :: Swedish = %d\n", head->Lang[i], (strcmp(head->Lang[i] , "Swedish")));
            if (strcmp(head->Lang[i] , usrIn) == 0)
            {
                
                printf("%s %d\n", head->Title, head->Year);
                break;
            }
            
        }
        head = head->next;
    }
    
    
    
    
}
void queryRating(struct movie * lst){
    struct movie * head = lst;
    struct movie * sel = lst;
    int * yearsPrinted ;
    yearsPrinted = (int*) calloc(125, sizeof(int));  
      //int year = 2008;
    int yearAlreadyPritnedFlag = 0;
    for (int i = 0; (i < 125) && (lst != NULL); i++)
    {
        //printf("? %d\n", head->Year);
        while (head != NULL)
        {
            for (int j = 0; j < 125; j++)
            {
                //printf("%d",yearsPrinted[j]);
                //printf("%d :: %d --> %d\n", yearsPrinted[j], head->Year, (yearsPrinted[j] == head->Year));
                if (yearsPrinted[j] == lst->Year)
                { 
                    yearAlreadyPritnedFlag = 1;
                    //printf("year already printed : %d ? %d\n",yearsPrinted[j], head->Year);
                }
            }
            //printf(":: %d\n",i);

            //printf("%d :: %d\n", lst->Year, head->Year);
            if(head->Year == lst->Year && sel->Rating <= head->Rating){
                sel = head;
                //printf("%s\n", sel->Title);
                
            }
            
            //printf("%s %0.1f:: %s %0.1f\n", head->Title, head->Rating, sel->Title, sel->Rating);
            head = head->next;
        }
        
        if (yearAlreadyPritnedFlag == 0)
        {
            printf("%d %0.1f %s\n", sel->Year, sel->Rating, sel->Title);
            yearsPrinted[i] = sel->Year;
        }

        lst = lst->next;
        head = lst;
        sel = lst;
        yearAlreadyPritnedFlag = 0;
    }
    free(yearsPrinted);
}




int main(int argc, char *argv[])
{
    int numOfItems = 0;
    if (argc < 2)
    {
        printf("You must provide the name of the file to process\n");
        printf("Example usage: ./movies movies_sample1.csv\n");
        return EXIT_FAILURE;
    }
    struct movie *list = processFile(argv[1], &numOfItems);
    printf("Processed file %s and parsed data for %d movies\n", argv[1], numOfItems);
    //printMovieList(list);
    //TODO: Write other functionality, 
    int exit = 0;
    int usrIn ;
    
    


    while (exit == 0)
    {
        switch (printMenu())
        {
        case 1: //Show movies released in the specified Year
            printf("Enter the Year for which you want to see movies: ");
            scanf("%d", &usrIn);
            getYear(usrIn,list);
            break;
        case 2: //Show highest rated movie for each Year
            queryRating(list);
            break;
        case 3: //Show the Title and Year of release of all movies in a specific language
            lang(list);
            break;
        case 4: //Exit from the program
            freeMovies(list);
            exit = 1;
            break;
        default:
        printf("You entered an incorrect choice. Try again.\n");
            break;
        }
    }
    //freeMovies(list);
    //TODO: Test Outcome
    
    return EXIT_SUCCESS;
    }