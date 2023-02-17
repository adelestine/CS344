#include "header.h"

/*
 *  This function gets user input and prints it to the screen
 *  @return 0
 *  
*/
int getUserInput(){
    printf(":");
    char input[100];
    clearscanf();
    scanf("%s", input);
    printf("%s", input);
    return 0;
}
/*
 * This function clears the scanf buffer
 *  @return void
*/
void clearscanf(){
    int c;
    while((c = getchar()) != '\n' && c != EOF);
}







/*
 *  This function is the main function
 *  @return 0
 *
*/
int main()
{
    getUserInput();
    

}
