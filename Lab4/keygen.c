#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>



/* the main function
 * generates a random key of the specified length with pre-defined characters
 */
int main(int argc, char *argv[]){
    srand(time(NULL)); //seed the random number generator
    if(argc != 2){
        fprintf(stderr, "Usage: keygen keylength, where keylength is the length of the key to generate");
        exit(1);
    }
    int keyLength = atoi(argv[1]);
    char key[keyLength +1];
    //allowed characters
    //A-Z and space

    int i;
    for(i = 0; i < keyLength; i++){ //generate the key from the allowed characters (use ASCII values)
        key[i] = (rand() % 27) + 65;
        if(key[i] == 91){ //91 is the ASCII value for [ and is not a valid character
            key[i] = 32; //32 is the ASCII value for space and is a valid character
        }
    }
    key[keyLength] = '\0';
    //Any re-directions should be done in the shell
    printf("%s\n", key);
    return 0;
}

