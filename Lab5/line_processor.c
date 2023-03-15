#include "header.h"

//process input as follows
/*
Thread 1, called the Input Thread, reads in lines of characters from the standard input.
Thread 2, called the Line Separator Thread, replaces every line separator in the input by a space.
Thread 3, called the Plus Sign thread, replaces every pair of plus signs, i.e., "++", by a "^".
Thread 4, called the Output Thread, write this processed data to standard output as lines of exactly 80 characters
*/


void moveData(struct Buffer *buffer1, struct Buffer *buffer2){
    for(int i = 0; i < buffer1->size; i++){
        buffer2->data[i] = buffer1->data[i];
    }
    buffer2->size = buffer1->size;
}

int main(int argc, char const *argv[])
{
    //usage: ./main < input.txt > output.txt
    //char *input = calloc(1000, sizeof(char));
    char lineSeparator = '\n';
    char * plusSign = "++";
    char output = '^';
    int lineLength = 80;
    int lineCount = 0;

//     struct Buffer buffer1, buffer2, buffer3, buffer4;
//     buffer1 = (struct Buffer){.data = calloc(1000, sizeof(char)), .size = 0};
//     buffer2 = (struct Buffer){.data = calloc(1000, sizeof(char)), .size = 0};
//     buffer3 = (struct Buffer){.data = calloc(1000, sizeof(char)), .size = 0};
//     buffer4 = (struct Buffer){.data = calloc(1000, sizeof(char)), .size = 0};
//     //do without threading first
//     //read in lines of characters from standard input
//     //input = fgets(input, 1000, stdin); cant use fgets because it stops at newline

//         for(int i = 0; i < 1000; i++){
//             buffer1.data[i] = fgetc(stdin);

//             if(buffer1.data[i] == EOF || (buffer1.data[i] == '\n' && buffer1.data[i-1] == 'P' && buffer1.data[i-2] == 'O' && buffer1.data[i-3] == 'T' && buffer1.data[i-4] == 'S')){
//                 //trim off the last 4 characters
//                 for(int j = i-4; j < i; j++){
//                     buffer1.data[j] = ' ';
//                 }
//                 break;
//             }

//         }

//     buffer1.size = strlen(buffer1.data);
//     // buffer1.data = realloc(buffer1.size, sizeof(char));
//     // buffer2.data = realloc(buffer1.size, sizeof(char));
//     // buffer3.data = realloc(buffer1.size, sizeof(char));
//     // buffer4.data = realloc(buffer1.size, sizeof(char));


//     moveData(&buffer1, &buffer2);

//     //test print input
//    // printf("%s\n", input);
//     //replace every line separator in the input by a space
//     //get size of input
    
//     //printf("size: %d\n", size);
    
//     for(int i = 0; i < buffer2.size; i++){
//         if(buffer2.data[i] == lineSeparator){
//             buffer2.data[i] = ' ';
//         }
//     }

//     moveData(&buffer2, &buffer3);
//     //replace every pair of plus signs, i.e., "++", by a "^"
//     for(int i = 0; i < buffer3.size; i++){

//         if(buffer3.data[i] == plusSign[0] && buffer3.data[i+1] == plusSign[1]){

//             buffer3.data[i] = output;
//             buffer3.data[i+1] = ' ';
//             for(int j = i+2; j < buffer3.size; j++){
//                 buffer3.data[j-1] = buffer3.data[j];
//             }
//         }
        
//     }
//     //resize input to remove null characters
//     //input = realloc(input, size);
//     moveData(&buffer3, &buffer4);
//     char *temp = calloc(80,sizeof(char));


//     //write this processed data to standard output as lines of exactly 80 characters
//     for(int i = 0; i < buffer4.size; i++){
//         if(lineCount == lineLength){
//             printf("%s", temp);
//             printf("\n");
//             lineCount = 0;
//         }
//         temp[lineCount] = buffer4.data[i];
//        // printf("%c", buffer4.data[i]);
//         lineCount++;
//     }

//     free(buffer1.data);
//     free(buffer2.data);
//     free(buffer3.data);
//     free(buffer4.data);
//     free(temp);
    //works as intended
    //now try with threading
    



    return 0;
}


