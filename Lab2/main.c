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





int main(int argc, char *argv[])
{
    char usrIn[20 + 1];
    int exit = 0;

    while (exit == 0) //main loop for selector
    {
        printMenu(1, usrIn); // print main menu and save in usrIn
        if (strcmp(usrIn, "1") == 0) //if user selects 1
        {
            printMenu(2, usrIn); //print file selector menu and save in usrIn
            printf("usrIn: %s\n",usrIn);
            if (strcmp(usrIn, "1") == 0) //if user selects 1
            {
                /* code */
            }
            else if (strcmp(usrIn, "2") == 0) //if user selects 2
            {
                /* code */
            }
            else if (strcmp(usrIn, "3") == 0) //if user selects 3
            {
                /* code */
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
    return EXIT_SUCCESS;
    //free(usrIn);
}