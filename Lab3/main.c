#include "header.h"

/*
 *  This function gets user input and prints it to the screen
 *  @return 0
 *  
*/
int getUserInput(){
    printf(":");
    char input[100];
    fgets(input, 100, stdin);
    printf("%s", input);
    return 0;
}







int main()
{
    getUserInput();
    

}
