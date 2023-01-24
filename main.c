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
    char *title; // title of Movie
    char *Lang[5]; // array of Languages the movie could be in
    int langLeng; // Length of the language array
    double rating; // rating of the movie
    int year; // year that the movie was released 
    struct movie *next; // a pointer to the next element in the Linked
};

// struct titleAndRating {
// 	char* title;
// 	double rating;
// };

struct dispVals {
	int year;
	char* title;
	double rating;
	//struct titleAndRating titleRating; // A struct to refrence just titles and ratings of a movie
	struct dispVals* next;
};

struct dispVals* createKeysValue(struct movie* list) {
	struct dispVals* temp = malloc(sizeof(struct dispVals));
	temp->title = malloc(strlen(list->title));
	strcpy(temp->title, list->title);
	temp->rating = list->rating;
	temp->year = list->year;
	temp->next = NULL;
	return temp;
}


void breakupLang(struct movie * currMovie, char * token){
    // int numOfLang = 0;
    // char * dtokenEnd;
    // char * langTok = strtok_r(dtoken, ";", &dtokenEnd);
    // while (langTok != NULL && numOfLang < 5){ // 5 is the max number of languages
    //     currMovie->Lang[numOfLang] = calloc(strlen(langTok) +1, sizeof(currMovie->Lang[numOfLang]));
    //     int sL = strlen(langTok) - 1; // string max length
    //     //3 cases
    //     // if only 1 language ----> IE: [English]
    //     // then both ends must be trimmed
    //     if(langTok[0] == '[' && langTok[sL] == ']'){
    //         filterChar(langTok, currMovie->Lang[numOfLang], '[');
    //         filterChar(langTok, currMovie->Lang[numOfLang], ']');
    //     }else if(langTok[0] == '['){ // trims First Language
    //         filterChar(langTok, currMovie->Lang[numOfLang], '[');
            
    //     }else if(langTok[sL] == ']'){
    //         filterChar(langTok, currMovie->Lang[numOfLang], ']');
    //     }else{
    //         strcpy(currMovie->Lang[numOfLang], langTok);
    //     }
    //     langTok = strtok_r(NULL, ";", &dtokenEnd);
    //     numOfLang++;
    // }

    // currMovie->langLeng = numOfLang;
    int languageIter = 0;
	char* langPtr;
	// Tokenize language array, which is in the following format: [language1<char*>;language2<char*>;...language5<char*>]
	char* languageToken = strtok_r(token, ";", &langPtr);
	// Loop through each language, from 1 - 5 languages, and handle each case:
	while (languageToken != NULL && languageIter < 5) {
		// Create character array for this language/
		currMovie->Lang[languageIter] = calloc(strlen(languageToken) + 1, sizeof(currMovie->Lang[languageIter]));
		int s = strlen(languageToken) - 1;
		// If one language, format is [some_language]. Filter out brackets
		if (languageToken[0] == '[' && languageToken[s] == ']') {
			filterChar(languageToken, currMovie->Lang[languageIter], '[');
			filterChar(currMovie->Lang[languageIter], currMovie->Lang[languageIter], ']');
		}
		// 1st language being processed.
		else if (languageToken[0] == '[') {
			filterChar(languageToken, currMovie->Lang[languageIter], '[');
		}
		// Last language being processed.
		else if (languageToken[s] == ']') {
			filterChar(languageToken, currMovie->Lang[languageIter], ']');
		}
		// All other cases (processing languages in-between)
		else {
			strcpy(currMovie->Lang[languageIter], languageToken);
		}
		languageToken = strtok_r(NULL, ";", &langPtr);
		languageIter += 1;
	}
	currMovie->langLeng = languageIter;
}

/* Parse the current line which is comma delimited and create a
*  movie struct with the data in each line
*/
struct movie *createMovie(char *currLine)
{
    struct movie *currMovie = malloc(sizeof(struct movie));
    currMovie->langLeng = 0;
    char *saveptr;
    char *token;
    char *endToken;


    // The first token is the Title
    token = strtok_r(currLine, ",", &saveptr);
    currMovie->title = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currMovie->title, token);

    // The next token is the Year
    token = strtok_r(NULL, ",", &saveptr);
    currMovie->year = atoi(token);

    //Lang
    token = strtok_r(NULL, ",", &saveptr);
    breakupLang(currMovie,token);


    // The last token is the Rating
    token = strtok_r(NULL, ",", &saveptr);
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

    // The head of the linked list
    struct movie *head = NULL;
    // The tail of the linked list
    struct movie *tail = NULL;

    //valid file?
    len = 256;
    if((nread = getline(&currLine, &len, movieFile)) == -1){
        printf("Non Valid / Empty File\n");
        return head;
    }
    // Read the file line by line
    while ((nread = getline(&currLine, &len, movieFile)) != -1)
    {
        // Get a new movie node corresponding to the current line
        struct movie * newNode = createMovie(currLine);

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
        *i +=1;
    }
    free(currLine);
    fclose(movieFile);
    return head;
}

void filterChar(char* source, char* destination, char comparator)
{
	// Loop through characters, only copy if character != comparator
	while (*source) { // source != '\0'
		if (*source != comparator) {
			*destination++ = *source;
		}
		source += 1;
	}
	*destination = '\0'; // Null terminate

}

// Used to validate user integer input. Assumes valid integer input.
int validateInputInt(const char* menu, const int lbound, const int ubound) {
	int retVal;
	printf("%s", menu);
	int isValid = scanf("%d", &retVal);
	// Reprompt user until valid integer is entered.
	// Doesn't catch number then character such as 3a, only reads 3 and discards a
	while (isValid == 0 || retVal < lbound || retVal > ubound) {
		printf("\nYou entered an incorrect choice. Try again.\n\n");
		flushStdin();
		printf("%s", menu);
		isValid = scanf("%d", &retVal);
	};
	flushStdin();
	return retVal;
};

// Flush STDIN buffer
void flushStdin(void) {
	int ch;
	while (((ch = getchar()) != '\n') && (ch != EOF));
}


void freeMovies(struct movie* list) {
	struct movie* temp;
	while (list) {
		temp = list;
		list = list->next;
		free(temp->title);
		// Free each language
		for (int i = 0; i < 5; ++i) {
			free(temp->Lang[i]);
		}
		free(temp);
	}
};

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

struct dispVals* createKeysValueList(struct movie* list) {
	// Create first node in list, increment movie pointer to next node
	struct dispVals* head = createKeysValue(list);
	struct dispVals* tail = head;
	struct movie* movieList = list->next;

	// Loop through each remaining movie node and create and add <year, <title, rating>> node to list if unique year.
	// Else swap info for existing node if new rating is higher.
	while (movieList) {
		// Try to find matching/existing movie year, and replace with highest rating if needed
		while (tail->next) {
			// If node with existing year exists, set to higher rating
			if (movieList->year == tail->year) {
				if (movieList->rating > tail->rating) {
					// Ensure adequate memory in source, then copy
					tail->title = realloc(tail->title, strlen(movieList->title));
					strcpy(tail->title, movieList->title);
					tail->rating = movieList->rating;
				}
				break;
			}
			tail = tail->next;
		}
		// Loop above stops without checking last node (since tail->next == NULL). Need to check last node in tail list.
		if (movieList->year == tail->year) {
			if (movieList->rating > tail->rating) {
				// Ensure adequate memory in source, then copy
				tail->title = realloc(tail->title, strlen(movieList->title));
				strcpy(tail->title, movieList->title);
				tail->rating = movieList->rating;
			}
		}
		else {
			// Create new node and add to list since current year does not exist
			struct dispVals* newNode = createKeysValue(movieList);
			tail->next = newNode;
		}
		tail = head; // rest tail to the beginning
		movieList = movieList->next; //move to next movie node
	}
	return head;
}

// void getYear(int yearGiven, struct movie *head){

//     int numMovies = 0;
//     while (head != NULL)
//     {
//         if(head->year == yearGiven){
//             printf("%s \n", head->title);
//             numMovies++;
//         }
//         head = head->next;
        
//     }
//     if(numMovies == 0){
//         printf("No data about movies released in the year %d\n \n", yearGiven);
//     }
    
// }

// Display title and highest rating for each year
void printKeysValue(struct dispVals* list) {
	struct dispVals* head = list;
	while (head) {
		printf("%d %0.1f %s\n", head->year, head->rating, head->title);
		head = head->next;
	}
}

void freeKeysValue(struct dispVals* list) {
	struct dispVals* temp;
	while ((temp = list) != NULL) {
		list = list->next;
		free(temp->title);
		free(temp);
	}
	list = NULL;
};

void printMoviesByLanguage(struct movie* list, char* lang) {
	struct movie* temp = list;
	int exists = 0;
	while (temp != NULL) {
		for (int i = 0; i < temp->langLeng; ++i) {
			if (strcmp(temp->Lang[i], lang) == 0) {
				printf("%d %s\n", temp->year, temp->title);
				exists = 1;
				break;
			}
		}
		temp = temp->next;
	};
	if (!exists) {
		printf("No data about movies released in %s\n", lang);
	}
}


void processMovieLanguages(struct movie* currMovie, char* token) {
	int languageIter = 0;
	char* langPtr;
	// Tokenize language array, which is in the following format: [language1<char*>;language2<char*>;...language5<char*>]
	char* languageToken = strtok_r(token, ";", &langPtr);
	// Loop through each language, from 1 - 5 languages, and handle each case:
	while (languageToken != NULL && languageIter < 5) {
		// Create character array for this language/
		currMovie->Lang[languageIter] = calloc(strlen(languageToken) + 1, sizeof(currMovie->Lang[languageIter]));
		int s = strlen(languageToken) - 1;
		// If one language, format is [some_language]. Filter out brackets
		if (languageToken[0] == '[' && languageToken[s] == ']') {
			filterChar(languageToken, currMovie->Lang[languageIter], '[');
			filterChar(currMovie->Lang[languageIter], currMovie->Lang[languageIter], ']');
		}
		// 1st language being processed.
		else if (languageToken[0] == '[') {
			filterChar(languageToken, currMovie->Lang[languageIter], '[');
		}
		// Last language being processed.
		else if (languageToken[s] == ']') {
			filterChar(languageToken, currMovie->Lang[languageIter], ']');
		}
		// All other cases (processing languages in-between)
		else {
			strcpy(currMovie->Lang[languageIter], languageToken);
		}
		languageToken = strtok_r(NULL, ";", &langPtr);
		languageIter += 1;
	}
	currMovie->langLeng = languageIter;
}

/*
*   Process the file provided as an argument to the program to
*   create a linked list of movie structs and print out the list.
*   Compile the program as follows:
*       gcc --std=gnu99 -o movies main.c
*/




int main(int argc, char *argv[])
{
    int menuChoice;
	int numLines = 0;
    if (argc < 2)
    {
        printf("You must provide the name of the file to process\n");
        printf("Example usage: ./movies movies_sample1.csv\n");
        return EXIT_FAILURE;
    }
    struct movie *list = processFile(argv[1], &numLines);
    printf("Processed file %s and parsed data for %d movies\n", argv[1], numLines);
    //printMovieList(list);
    //TODO: Write other functionality, 
    int exit = 0;
    
    


    while (exit == 0)
    {
        switch (printMenu())
        {
        case 1 ://Show movies released in the specified year
        {
           char* s = "Enter the year for which you want to see movies: ";
			// No input validation needed
			int caseChoice = validateInputInt(s, 1000, 9999);
			int exists = 0;
			// Loop through movie list and only print out title with matching year
			struct movie* temp = list;
			while (temp != NULL) {
				if (temp->year == caseChoice) {
					printf("%s\n", temp->title);
					exists = 1;
				};
				temp = temp->next;
			};
			if (!exists) {
				printf("No data about movies released in the year %d\n", caseChoice);
			}
            break;
        } 

            
        case 2: //Show highest rated movie for each year
        {
            struct dispVals* temp = createKeysValueList(list);
            printKeysValue(temp);
            freeKeysValue(temp);
            break;
        }
        case 3: //Show the title and year of release of all movies in a specific language
        {
            char caseChoice[20];
            printf("Enter the language for which you want to see movies: ");
            scanf("%s", caseChoice);
            printMoviesByLanguage(list, caseChoice);
            break;
        }
        case 4: //Exit from the program
            exit = 1;
            break;
        default:
        printf("You entered an incorrect choice. Try again.\n");
            break;
        }
        printf("\n");
    }
    freeMovies(list);
    //TODO: Test Outcome
    
    return EXIT_SUCCESS;
}
