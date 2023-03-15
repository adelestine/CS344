#include "header.h"

//process input as follows
/*
Thread 1, called the Input Thread, reads in lines of characters from the standard input.
Thread 2, called the Line Separator Thread, replaces every line separator in the input by a space.
Thread 3, called the Plus Sign thread, replaces every pair of plus signs, i.e., "++", by a "^".
Thread 4, called the Output Thread, write this processed data to standard output as lines of exactly 80 characters
*/








//buffer 1 shared resouces between b1 and b2
struct eLetter buffer1;
int inputDone = 0;
// Initialize the mutex for buffer 1
pthread_mutex_t mutex_1 = PTHREAD_MUTEX_INITIALIZER;
// Initialize the condition variable for buffer 1
pthread_cond_t full_1 = PTHREAD_COND_INITIALIZER;
pthread_cond_t done_1 = PTHREAD_COND_INITIALIZER;

struct eLetter buffer2;
int lineSepDone = 0;

pthread_mutex_t mutex_2 = PTHREAD_MUTEX_INITIALIZER;
// Initialize the condition variable for buffer 2
pthread_cond_t full_2 = PTHREAD_COND_INITIALIZER;

pthread_cond_t done_2 = PTHREAD_COND_INITIALIZER;

struct eLetter buffer3;
int plusSignDone = 0;

pthread_mutex_t mutex_3 = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t full_3 = PTHREAD_COND_INITIALIZER;

pthread_cond_t done_3 = PTHREAD_COND_INITIALIZER;
int outputDone = 0;



void buffer1Write(char *data, int size){
    pthread_mutex_lock(&mutex_1);
    strcpy(buffer1.data, data);
    //buffer1.data = data;
    buffer1.size = size;

    //printf("data1: %s \n", buffer1.data);
    pthread_cond_signal(&full_1);
    pthread_mutex_unlock(&mutex_1);
}
void buffer2Write(char *data, int size){
    pthread_mutex_lock(&mutex_2);
    //buffer2.data = data;
    strcpy(buffer2.data, data);
    buffer2.size = size;
    //printf("data2: %s \n", buffer2.data);
    pthread_cond_signal(&full_2);
    pthread_mutex_unlock(&mutex_2);
}
void buffer3Write(char *data, int size){
    pthread_mutex_lock(&mutex_3);
    //buffer3.data = data;
    strcpy(buffer3.data, data);
    buffer3.size = size;
    //printf("data3: %s \n", buffer3.data);
    pthread_cond_signal(&full_3);
    pthread_mutex_unlock(&mutex_3);
}

void buffer1Read(char *data, int *size){
    pthread_mutex_lock(&mutex_1);
    //*data = buffer1.data;
    //wait until buffer1 is full
    while(buffer1.size == 0){
        pthread_cond_wait(&full_1, &mutex_1);
    }
    strcpy(data, buffer1.data);
    *size = buffer1.size;
    //free(buffer1.data);
    pthread_mutex_unlock(&mutex_1);
}
void buffer2Read(char *data, int *size){
    pthread_mutex_lock(&mutex_2);
    //*data = buffer2.data;
    //wait until buffer2 is full
    while(buffer2.size == 0){
        pthread_cond_wait(&full_2, &mutex_2);
    }
    strcpy(data, buffer2.data);
    *size = buffer2.size;
    //free(buffer2.data);
    pthread_mutex_unlock(&mutex_2);
}
void buffer3Read(char *data, int *size){
    pthread_mutex_lock(&mutex_3);
    //*data = buffer3.data;
    //wait until buffer3 is full
    while(buffer3.size == 0){
        pthread_cond_wait(&full_3, &mutex_3);
    }
    strcpy(data, buffer3.data);
    *size = buffer3.size;
    //free(buffer3.data);
    pthread_mutex_unlock(&mutex_3);
}


void getUserIn(struct eLetter *bufferin){
    for(int i = 0; i < 1000; i++){
            bufferin->data[i] = fgetc(stdin);
            if(bufferin->data[i] == EOF || (bufferin->data[i] == '\n' && bufferin->data[i-1] == 'P' && bufferin->data[i-2] == 'O' && bufferin->data[i-3] == 'T' && bufferin->data[i-4] == 'S')){
                //trim off the last 4 characters
                for(int j = i-4; j < i; j++){
                    bufferin->data[j] = '\0';
                }
                bufferin->size = i-4;
                break;
            }
    }
}




void * input(void *arg){
    struct eLetter bufferin;
    //bufferin.data = malloc(1000);
    bufferin.size = 0;
    
    getUserIn(&bufferin); //fills the buffer with user input
    buffer1Write(bufferin.data, bufferin.size);
    //test print bufferin





    //free(bufferin.data); //needs to be done outside of the function?
    return NULL;
}

void * lineSeparator(void *arg){
    struct eLetter bufferin;
    //bufferin.data = calloc(1000, sizeof(char));
    bufferin.size = 0;
    buffer1Read(bufferin.data, &bufferin.size);
    //realloc(bufferin.data, bufferin.size);
    for(int i = 0; i < bufferin.size; i++){
        if(bufferin.data[i] == '\n'){
            bufferin.data[i] = ' ';
        }
    }
    buffer2Write(bufferin.data, bufferin.size);

    return NULL;
}

void * plusSign(void *arg){
    struct eLetter bufferin;
    //bufferin.data = malloc(1000);
    bufferin.size = 0;
    buffer2Read(bufferin.data, &bufferin.size);
    //realloc(bufferin.data, bufferin.size);
    for(int i = 0; i < bufferin.size; i++){
        if(bufferin.data[i] == '+' && bufferin.data[i+1] == '+'){
            bufferin.data[i] = '^';
            bufferin.data[i+1] = '\0';
            for(int j = i+1; j < bufferin.size; j++){
                bufferin.data[j] = bufferin.data[j+1];
            }
        }
    }
    buffer3Write(bufferin.data, bufferin.size);
    //free(bufferin.data);
    return NULL;
}

void * output(void *arg){
    struct eLetter bufferin;
    //bufferin.data = malloc(1000);
    bufferin.size = 0;
    char * temp = calloc(80, sizeof(char));
    //char temp[80];
    buffer3Read(bufferin.data, &bufferin.size);
    //realloc(bufferin.data, bufferin.size);
    for(int i = 0; i < bufferin.size; i++){
        if(i % 80 == 0 && i != 0){
            printf("%s", temp);
            printf("\n");
            fflush(stdout);
        }
        temp[i % 80] = bufferin.data[i];
    }
    //free(bufferin.data);
    free(temp);
}





/*

void moveData(struct eLetter *buffer1, struct eLetter *buffer2){
    for(int i = 0; i < buffer1->size; i++){
        buffer2->data[i] = buffer1->data[i];
    }
    buffer2->size = buffer1->size;
}

void * getUserIn(struct eLetter *bufferin){
    for(int i = 0; i < 1000; i++){
            bufferin->data[i] = fgetc(stdin);

            if(bufferin->data[i] == EOF || (bufferin->data[i] == '\n' && bufferin->data[i-1] == 'P' && bufferin->data[i-2] == 'O' && bufferin->data[i-3] == 'T' && bufferin->data[i-4] == 'S')){
                //trim off the last 4 characters
                for(int j = i-4; j < i; j++){
                    bufferin->data[j] = ' ';
                }
                break;
            }
        }

    bufferin->size = strlen(bufferin->data);
}





*/


int main(int argc, char const *argv[])
{
    srand(time(0));
    pthread_t inputThread, lineSeparatorThread, plusSignThread, outputThread;
    pthread_create(&inputThread, NULL, input, NULL);
    pthread_create(&lineSeparatorThread, NULL, lineSeparator, NULL);
    pthread_create(&plusSignThread, NULL, plusSign, NULL);
    pthread_create(&outputThread, NULL, output, NULL);
    //pthread_cond_wait(&cond_1, &mutex_1);
    //wait for all threads to finish
    pthread_join(inputThread, NULL);
    pthread_join(lineSeparatorThread, NULL);
    pthread_join(plusSignThread, NULL);
    pthread_join(outputThread, NULL);
    //free(buffer1.data);
    //free(buffer2.data);
    //free(buffer3.data);
    return 0;
}











    //usage: ./main < input.txt > output.txt
    //char *input = calloc(1000, sizeof(char));

   // int lineCount = 0;
    /*
    struct eLetter buffer1, buffer2, buffer3, buffer4;
    buffer1 = (struct eLetter){.data = calloc(1000, sizeof(char)), .size = 0};
    buffer2 = (struct eLetter){.data = calloc(1000, sizeof(char)), .size = 0};
    buffer3 = (struct eLetter){.data = calloc(1000, sizeof(char)), .size = 0};
    buffer4 = (struct eLetter){.data = calloc(1000, sizeof(char)), .size = 0};
    //do without threading first
    //read in lines of characters from standard input
    //input = fgets(input, 1000, stdin); cant use fgets because it stops at newline
    getUserIn(&buffer1);

    buffer1.size = strlen(buffer1.data);
    // buffer1.data = realloc(buffer1.size, sizeof(char));
    // buffer2.data = realloc(buffer1.size, sizeof(char));
    // buffer3.data = realloc(buffer1.size, sizeof(char));
    // buffer4.data = realloc(buffer1.size, sizeof(char));


    moveData(&buffer1, &buffer2);

    //test print input
   // printf("%s\n", input);
    //replace every line separator in the input by a space
    //get size of input
    
    //printf("size: %d\n", size);
    
    for(int i = 0; i < buffer2.size; i++){
        if(buffer2.data[i] == LINE_SEP){
            buffer2.data[i] = ' ';
        }
    }

    moveData(&buffer2, &buffer3);
    //replace every pair of plus signs, i.e., "++", by a "^"
    for(int i = 0; i < buffer3.size; i++){

        if(buffer3.data[i] == PLUS_SIGN[0] && buffer3.data[i+1] == PLUS_SIGN[1]){

            buffer3.data[i] = CHAR_;
            buffer3.data[i+1] = ' ';
            for(int j = i+2; j < buffer3.size; j++){
                buffer3.data[j-1] = buffer3.data[j];
            }
        }
        
    }
    //resize input to remove null characters
    //input = realloc(input, size);
    moveData(&buffer3, &buffer4);
    char *temp = calloc(80,sizeof(char));


    //write this processed data to standard output as lines of exactly 80 characters
    for(int i = 0; i < buffer4.size; i++){
        if(lineCount == SIZE){
            printf("%s", temp);
            printf("\n");
            lineCount = 0;
        }
        temp[lineCount] = buffer4.data[i];
       // printf("%c", buffer4.data[i]);
        lineCount++;
    }

    free(buffer1.data);
    free(buffer2.data);
    free(buffer3.data);
    free(buffer4.data);
    free(temp);
    */
    //works as intended
    //now try with threading
    


