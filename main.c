// If you are not compiling with the gcc option --std=gnu99, then
// uncomment the following line or you might get a compiler warning
//#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"



/* struct for movie information */
struct movie
{
    char *title;
    char *Lang;
    double rating;
    int year;
    struct movie *next;
};




/* Parse the current line which is comma delimited and create a
*  movie struct with the data in each line
*/
struct movie *createMovie(char *currLine)
{
    struct movie *currMovie = malloc(sizeof(struct movie));

    // For use with strtok_r
    char *saveptr;
    char *saveptr2;

    char *token;
    char *dtoken;
    char *endToken;


    // The first token is the Title
    token = strtok_r(currLine, ",", &saveptr);
    currMovie->title = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currMovie->title, token);

    // The next token is the Year
    token = strtok_r(NULL, ",", &saveptr);
    //currMovie->year = calloc(1, sizeof(int));
    currMovie->year = atoi(token);
   // strcpy(currMovie->year, token);

    // The next token is the Languages *****COMPLEX!!!
   // token = strtok_r(NULL, ",", &saveptr);
    //TODO Test


    /*
    currMovie->Lang[0] = strtok(token, ";");
    dtoken = strtok_r(NULL, "[" ,&saveptr); // might be [ or ]
    while(dtoken !=0){
        dtoken = strtok(NULL, ";");
        currMovie->Lang[i] =calloc(strlen(dtoken) +1, sizeof(char));
        strcpy(currMovie->Lang[i], dtoken);
        i++;
    }
    */


    


    // int y = 0;
    dtoken = strtok_r(NULL, ",", &saveptr);
    // //find number of semicolins, allocate e
    
    // currMovie->Lang = calloc(strlen(dtoken)+1, sizeof(char));
    // while(dtoken != NULL){
    //     dtoken = strtok_r(NULL, ";", &saveptr2); // little smily face
    //     currMovie->Lang
    //     currMovie->Lang[y] =calloc(strlen(dtoken) +1, sizeof(char));
    //     strcpy(currMovie->Lang[y], dtoken);
    //     y++;
    // }
    
    
    currMovie->Lang = calloc(strlen(dtoken) + 1, sizeof(char));
    strcpy(currMovie->Lang, dtoken);

    //token = strtok(currLine, "]")
    // The last token is the Rating
    token = strtok_r(NULL, "\n", &saveptr);
    //currMovie->rating = calloc(8, sizeof(double));
    currMovie->rating = strtod(token, &endToken);
    //strcpy(currMovie->rating, token);



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

/*
* Print data for the given movie
*/
void printMovie(struct movie *aMovie){
  printf("%s, %d %s, %.1f\n", aMovie->title,
  aMovie->year,
  aMovie->Lang,
  aMovie->rating);
}
/*
* Print the linked list of movies
*/
void printMovieList(struct movie *list)
{
    while (list != NULL)
    {
        printMovie(list);
        list = list->next;
    }
}
void freeMovies(struct movie *head){ // found on Stack Overflow
    struct movie* tmp; 
    while (head != NULL)
    {

        tmp = head;
        head = head->next;
        free(tmp->Lang);
        //free(Mlist->rating);
        free(tmp->title);
        free(tmp);
        //free(Mlist->year);
        //free(Mlist->next);
    }

    //free(Mlist);
    //free(savedNext);
    
}
int printMenu(){
    int output;
    printf("1. Show movies released in the specified year\n");
    printf( "2. Show highest rated movie for each year\n");
    printf( "3. Show the title and year of release of all movies in a specific language\n");
    printf(  "4. Exit from the program\n \n");

    printf("Enter a choice from 1 to 4: ");
    scanf("%d", &output);
    return output;


}

void getYear(int yearGiven, struct movie *head){

    int numMovies = 0;
    while (head != NULL)
    {
        if(head->year == yearGiven){
            printf("%s \n", head->title);
            numMovies++;
        }
        head = head->next;
        
    }
    if(numMovies == 0){
        printf("No data about movies released in the year %d\n \n", yearGiven);
    }
    
}

void queryRating(struct movie *head){
    struct movie * lookingAt;
    struct movie* top = head;
    lookingAt = head;
    int* processedYears;
    int size = 130;
    int i;
    processedYears = (int*)calloc(size,sizeof(int));
    while (lookingAt != NULL)
    {
        for(i = 0; i < size; i++){
            while(1){
                if(processedYears[i] == lookingAt->year){
                    lookingAt = lookingAt->next;
                    i = 0;
                }else{
                    break;
                } 
        }
    }
    head = top;
    while(head != NULL){
        if((head->rating > lookingAt->rating) && (head->year == lookingAt->year)){
            lookingAt = head;
        }
        head = head->next;
    }

    
    //printf("%d %.1f %s \n", lookingAt->year, lookingAt->rating, lookingAt->title);
    int j;
    for (j = 0; j < size; j++)
    {
        printf("data @ %d, :%d\n", j, processedYears[size-1]);

    }
    
    processedYears[size-1] = lookingAt->year;
    //printf("\n%d\n", processedYears[size-1]);
    //size++;
    }
    






    //grab 1st title, then compare agiant everything
    // printf("0");
    // struct movie* tmp; 
    // struct movie* top = head;
    // int yearCurrent = 0;
    // 
    // 
    // int j = 0;
    // int exit = 0;
    // printf("1");
    // while (exit != 1)
    // {
    //      printf("2");
         
    //      processedYears = (int*)calloc(size,sizeof(int));
    //      head = top;
    //      tmp = head;
         
    //     while (yearCurrent != 0)
    //     {
    //         break;
    //         if (tmp->year == processedYears[j] && processedYears[j] != 0 && tmp->next == NULL)
    //         {
    //             exit = 1;
    //             break;
    //         }else if(tmp->year == processedYears[j] && processedYears[j] != 0 ){
    //             tmp = tmp->next;
    //             j++;
    //         }else{
    //             yearCurrent = 0;
    //         }
            
    //     }
    //      if(exit ==1){
    //          break;
    //      }
    //     yearCurrent = tmp->year;
    //     while (head != NULL)
    //     {
    //         if((head->rating > tmp->rating) && (head->year == tmp->year)){
    //             tmp = head;
    //         }
    //     }
    //     printf("%d %.1f %s \n", tmp->year, tmp->rating, tmp->title);
    //     processedYears[size-1] = tmp->year;
    //     size++;
    //     j++;
    // }
    


    // //grab first title, look at year, 
    // //search all others for if they have a better rating and same year
    // //print
    free(processedYears);
}
/*
*   Process the file provided as an argument to the program to
*   create a linked list of movie structs and print out the list.
*   Compile the program as follows:
*       gcc --std=gnu99 -o movies main.c
*/
void lang(struct movie *head){
    char * saveptr;
    char * tok =  strtok_r(head->Lang, ";[]", &saveptr);
    char usrInLang[21];
    printf("Enter the language for which you want to see movies:\n");
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    scanf("%20[^\n]", usrInLang);


    while (head->next != NULL)
    {   
        //temp = strcmp(tok, usrInLang);
        //printf("Test1: %d == %s\n", strcmp(tok,usrInLang), usrInLang);
        
        printf("%d, %s", strlen(tok), tok);
        tok = strtok_r(NULL,";",&saveptr);
        // printf("%d, %s", strlen(tok), tok);
        // tok = strtok_r(NULL, ";[]", &saveptr);
        // printf("%d, %s", strlen(tok), tok);
        // tok = strtok_r(NULL, ";[]", &saveptr);
        // printf("%d, %s", strlen(tok), tok);S
        // while(strlen(tok) != ){
        //     if(strcmp(tok, usrInLang) == 0){
        //         printf("%d %s \n", head->year, head->title);
        //     }
        // tok = strtok_r(head->Lang, ";[]", &saveptr);
        // }
            


        head = head->next;

    }
    
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
        case 1: //Show movies released in the specified year
            printf("Enter the year for which you want to see movies: ");
            scanf("%d", &usrIn);
            getYear(usrIn,list);
            break;
        case 2: //Show highest rated movie for each year
            queryRating(list);
            break;
        case 3: //Show the title and year of release of all movies in a specific language
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
    
    //TODO: Test Outcome
    
    return EXIT_SUCCESS;
    }