#include "header.h"

/*
 *  This function gets user input and prints it to the screen
 *  @return 0
 *  
*/
int getUserInput(struct inputStr * inputStr){
    printf(":");
    char input[2048];
    fgets(input, 2048, stdin);
    int charInput = strlen(input);
    //printf("%d", charInput);
    
    inputStr->input = calloc(charInput +1, sizeof(char)); //uninitialized memory size of 8
    if (inputStr->input == NULL) {
        // Handle allocation failure
        return -1;
    }
    strcpy(inputStr->input, input);

    inputStr->length = charInput;
    inputStr->isBackground = false;
    inputStr->inputFile = NULL;
    inputStr->outputFile = NULL;
    inputStr->command = calloc(charInput +1, sizeof(char));
    inputStr->args = NULL;
    //loop thorugh input and count how many arguments there are
    int numArgs = 0;
    for (int i = 0; i < charInput; i++){
        if ((input[i] == ' ') && input[i+1] != ' ' && input[i+1] != '<' && input[i+1] != '>' && input[i+1] != '&'){ 
            numArgs = numArgs + 1;
        }
    }
    inputStr->argCount = numArgs+1;
    //printf("%d", numArgs);
    inputStr->args = calloc(numArgs + 1, sizeof(struct Link));
    if (inputStr->args == NULL) {
        // Handle allocation failure
        return -1;
    }


   


    return charInput;
}



/*
 *  This function sets the input file
 *  @return 1 if successful
 *  @return 0 if not successful
 *
*/
int setInputFile(struct inputStr * inputStr, int * i){
    int j = *i;
    inputStr->inputFile = calloc(inputStr->length, sizeof(char));      // allocate memory for the input file name
    for (j; j < inputStr->length; j++){                                 // loop through the input
        if (inputStr->input[j-1] == '<' && inputStr->input[j] == ' '){  // check if the previous character is < and the current character is a space
            j = j + 1;
            int k = 0;
            for (j; j < inputStr->length; j++){                         // loop through the name of the file
                if (inputStr->input[j] == ' ' || inputStr->input[j] == '\n'){ // check if the current character is a space or a new line and if so end reading file name
                    break;
                }
                else{
                    inputStr->inputFile[k] = inputStr->input[j];              // add the current character to the file name
                    k = k + 1;
                }
            }
            break;
        }
        else if (inputStr->input[j-1] == '<' && inputStr->input[j] != ' '){ // check if the previous character is < and the current character is not a space
            printf("Error: No space after <");                              // print error message
        }
    }
    return 1;
}


/*
 * This function sets the output file
 * @return 1 if successful
 * @return 0 if not successful
 * 
*/
int setOutputFile(struct inputStr * inputStr, int * i){
    int j = *i;
    inputStr->outputFile = calloc(inputStr->length, sizeof(char));      // allocate memory for the input file name
    for (j; j < inputStr->length; j++){                                 // loop through the input
        if (inputStr->input[j-1] == '>' && inputStr->input[j] == ' '){  // check if the previous character is < and the current character is a space
            j = j + 1;
            int k = 0;
            for (j; j < inputStr->length; j++){                         // loop through the name of the file
                if (inputStr->input[j] == ' ' || inputStr->input[j] == '\n'){ // check if the current character is a space or a new line and if so end reading file name
                    break;
                }
                else{
                    inputStr->outputFile[k] = inputStr->input[j];              // add the current character to the file name
                    k = k + 1;
                }
            }
            break;
        }
        else if (inputStr->input[j-1] == '>' && inputStr->input[j] != ' '){ // check if the previous character is < and the current character is not a space
            printf("Error: No space after <");                              // print error message
        }
    }
    return 1;
}



/*
 *  This function parses the input
 *  loops through the input and checks for < > and &
 *  @return 0
 *
*/
void parseInput(struct inputStr * inputStr){
    pidReplace(inputStr);
    //parse the input
    int i = 0;
    //check for < > and &
    while (i < inputStr->length){
        if (inputStr->input[i] == '<'){
            //set the input file
            setInputFile(inputStr, &i);
            //printf("%d", i);
        }
        else if (inputStr->input[i] == '>'){
            //set the output file
            setOutputFile(inputStr, &i);
        }
        else if (inputStr->input[i] == '&'){
            //set isBackground to true
            inputStr->isBackground = true;
        }
        i = i + 1;
    }
   
    //read Command

    // while (i < inputStr->length && inputStr->input[i] != ' '){
    //     //set the command
    //     inputStr->command[i] = inputStr->input[i];
    //     i = i + 1;
    // }
    //printf("Command: %s\n", inputStr->command);
    //read args
     i = 0  ;
    int j = 0;
    int k = 0;
    struct Link * link = inputStr->args;
    while (i < inputStr->length-1 && inputStr->input[i] != '\n' && inputStr->input[i] != '<' && inputStr->input[i] != '>' && inputStr->input[i] != '&'){
        if (inputStr->input[i] == ' '){
            //set the arg
            char * arg = calloc(i-j+1, sizeof(char));
            for (j; j < i; j++){
                arg[k] = inputStr->input[j];
                k = k + 1;
            }
            //printf("Arg: %s\n", arg);
            link->data = arg;
            link->next = calloc(1, sizeof(struct Link));
            link = link->next;
            k = 0;
            j = j +1;
        }else{
            
        }
        i = i + 1;
    }
    //set the last arg
    char * arg = calloc(i-j+1, sizeof(char));
    for (j; j < i; j++){
        arg[k] = inputStr->input[j];
        k = k + 1;
    }
    link->data = arg;
    link->next = NULL;

    strcpy(inputStr->command, inputStr->args->data);
    //remove first Arg
    // struct Link * temp = inputStr->args;
    // inputStr->args = inputStr->args->next;
    // free(temp);
    removeEmptyNodes(inputStr->args);
    


}

/*
 * This function removes empty nodes from the linked list
 * obtained from chatgpt.
 * @return 0
*/
void removeEmptyNodes(struct Link *head) {
    struct Link *prev = NULL;
    struct Link *current = head;

    while (current != NULL) {
        // check if the current node has empty data
        if (strlen(current->data) == 0) {
            // if so, remove it from the list
            if (prev != NULL) {
                // if the current node is not the first node, adjust the previous node's next pointer
                prev->next = current->next;
                free(current);
                current = prev->next;
            } else {
                // if the current node is the first node, adjust the head pointer
                head = current->next;
                free(current);
                current = head;
            }
        } else {
            // if the current node has non-empty data, move on to the next node
            prev = current;
            current = current->next;
        }
    }
}


/*
 * This function replaces any instances of $$ with the process id
 * @return 0
*/
void pidReplace(struct inputStr * inputStr){
    int i = 0;
    int j = 0;
    int k = 0;
    int pid = getpid();
    char * pidStr = calloc(10, sizeof(char));
    sprintf(pidStr, "%d", pid);
    int pidLength = strlen(pidStr);
    int inputLength = strlen(inputStr->input);
    char * newInput = calloc(inputLength + pidLength, sizeof(char));
    while (i < inputLength){
        if (inputStr->input[i] == '$' && inputStr->input[i+1] == '$'){
            //replace with pid
            for (j; j < i; j++){
                newInput[k] = inputStr->input[j];
                k = k + 1;
            }
            for (j = 0; j < pidLength; j++){
                newInput[k] = pidStr[j];
                k = k + 1;
            }
            i = i + 2;
            j = i;
        }else{
            i = i + 1;
        }
    }
    for (j; j < i; j++){
        newInput[k] = inputStr->input[j];
        k = k + 1;
    }
    inputStr->input = newInput;
    inputStr->length = strlen(inputStr->input);
}



//required built in commands
//cd
//exit
//status


/*
 *  This function checks if the command is a built in command
 *  @return 0 if the command is not a built in command
 *  @return 1 if the command is a built in command
 *
*/
int isBuiltInCommand(struct inputStr * inputStr){
    if (inputStr->command[0] == '#'){
        return 1;
    } else if (strcmp(inputStr->command, "cd") == 0){
        //change directory
        printf("cd\n");
        if (inputStr->args->next == NULL){
            chdir(getenv("HOME"));
        }else{
            chdir(inputStr->args->next->data);
        }
        return 1;
    }else if (strcmp(inputStr->command, "exit") == 0){
        //exit
        //printf("exit\n");
        inputStr->isExit = true;
        return 1;
    }else if (strcmp(inputStr->command, "status") == 0){
        //status
        //printf("status\n");
        //print status
        int status;
        if(WIFEXITED(status)){
            printf("exit value %d\n", WEXITSTATUS(status));
        }else if(WIFSIGNALED(status)){
            printf("terminated by signal %d\n", WTERMSIG(status));
        }
        return 1;
    }else{
        return 0;
    }
}

/* Status
 * This function prints the status of the last command
*/
// void status(){
//     //printf("status\n");
//     if(WIFEXITED(status)){
//         printf("exit value %d\n", WEXITSTATUS(status));
//     }else if(WIFSIGNALED(status)){
//         printf("terminated by signal %d\n", WTERMSIG(status));
//     }
// }


//to handle non built in commands
//fork
//exec
//waitpid

/*
 * This function handles non built in commands
 * 
 * 
 * 
*/
int runCommand(struct inputStr * inputStr){
    //save current FD
    int savedFD = dup(1);

    // printf("->runCommand\n");
    // printf("->Number of args: %d\n", inputStr->argCount);
    // printf("->Command: %s\n", inputStr->command);
    //printf("->Arg0: %s\n", inputStr->args->data);
    int stat;
    char * args[inputStr->argCount + 1];
    //char * testArgs[] = {"ls", NULL};

    //copy args to static array
    int i = 0;
    struct Link * link = inputStr->args;
    while (link != NULL){
        args[i] = link->data;
        link = link->next;
        i = i + 1;
    }
    args[i] = NULL;
    
    //print args
    // for (int i = 0; i < inputStr->argCount+1; i++){
    //     //printf("->Arg0%d: %s\n",i, testArgs[i]);
    //     printf("->Arg2.%d: %s\n",i, args[i]);
    // }
    

    //check to make sure not in backround
    if()
    




    int childID = fork();
    //if output file is specified

    




    // //fork
    if (childID == 0)
    {
        //printf("I am the child\n"); //RUNS ONCE
        //WANT



        if (inputStr->outputFile != NULL){
        printf("->outputFile: %s\n", inputStr->outputFile);

        //open file
        int targetFD = open(inputStr->outputFile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (targetFD == -1){
            printf("Error opening file\n");
            exit(1);
        }
        dup2(targetFD, 1);
    }
    //check for input file
    if (inputStr->inputFile != NULL){
        printf("->inputFile: %s\n", inputStr->inputFile);

        //open file
        int targetFD = open(inputStr->inputFile, O_RDONLY);
        if (targetFD == -1){
            printf("Error opening file\n");
            exit(1);
        }
        dup2(targetFD, 0);
    }
        if(execvp(inputStr->command, args) == -1)
        {
            printf("Error");
        }
        exit(1);
    }
    else
    {
        //printf("I am the parent\n"); //RUNS ONCE
        waitpid(-1, &stat, 0); //wait for child to finish
        //return to saved FD
        dup2(savedFD, 1);
    }
    // if( WIFEXITED(stat) )
    // {
    //     //printf("Child exited with code %d\n", WEXITSTATUS(stat));
    // }else if( WIFSIGNALED(stat) )
    // {
    //     //printf("Child was terminated by signal %d\n", WTERMSIG(stat));
    // }
    
    //printf("Only run by the parent\n"); //RUNS ONCE
    return 0;
}







/*

 ______________     _             _,-----------._        ___
|              |   (_,.-      _,-'_,-----------._`-._    _)_)
| THE _  _  _  |      |     ,'_,-'  ___________  `-._`.
| |  / \|_)| \ |     `'   ,','  _,-'___________`-._  `.`.
| |__\_/| \|_/ |        ,','  ,'_,-'     .     `-._`.  `.`.
|              |       /,'  ,','        >|<        `.`.  `.\
| OF THE  _ _  |      //  ,','      ><  ,^.  ><      `.`.  \\
| |_)||\|/_(_  |     //  /,'      ><   / | \   ><      `.\  \\
| | \|| |\_|_) |    //  //      ><    \/\^/\/    ><      \\  \\
|______________|   ;;  ;;              `---'              ::  ::
                   ||  ||              (____              ||  ||
 DOORS OF DURIN   _||__||_            ,'----.            _||__||_
                 (o.____.o)____        `---'        ____(o.____.o)
                   |    | /,--.)                   (,--.\ |    |
                   |    |((  -`___               ___`   ))|    |
                   |    | \\,'',  `.           .'  .``.// |    |
                   |    |  // (___,'.         .'.___) \\  |    |
                  /|    | ;;))  ____) .     . (____  ((\\ |    |\
                  \|.__ | ||/ .'.--.\/       `/,--.`. \;: | __,|;
                   |`-,`;.| :/ /,'  `)-'   `-('  `.\ \: |.;',-'|
                   |   `..  ' / \__.'         `.__/ \ `  ,.'   |
                   |    |,\  /,                     ,\  /,|    |
                   |    ||: : )          .          ( : :||    |
                  /|    |:; |/  .      ./|\,      ,  \| :;|    |\
                  \|.__ |/  :  ,/-    <--:-->    ,\.  ;  \| __,|;
                   |`-.``:   `'/-.     '\|/`     ,-\`;   ;'',-'|
                   |   `..   ,' `'       '       `  `.   ,.'   |
                   |    ||  :                         :  ||    |
                   |    ||  |                         |  ||    |
                   |    ||  |                         |  ||    |
                   |    |'  |            _            |  `|    |
                   |    |   |          '|))           |   |    |
                   ;____:   `._        `'           _,'   ;____:
                  {______}     \___________________/     {______}
              SSt |______|_______________________________|______|
*/






/*
 *  This function is the main function
 *  @return 0
 *
*/
int main(){
    //what needs to happen
    //get user input
    // -> handle for commnads < > and & 
    // only read & as the last character
    


    //combo of chatGPT and Copilot
    struct inputStr inputStr = {
        .input = NULL,
        .length = 0,
        .isBackground = false,
        .inputFile = NULL,
        .outputFile = NULL,
        .command = NULL,
        .args = NULL
    };
    


    while(true){
    getUserInput(&inputStr);
    parseInput(&inputStr);
    if(isBuiltInCommand(&inputStr) != true){
        runCommand(&inputStr);
    }
    if (inputStr.isExit == true)
    {
        break;
    }
    
    

    
    
    
    

    //print Input 




    //testing

    //only print if parent process
    // if(getpid() == 0){
    
    // printf("#Input: %s\n", inputStr.input);
    // printf("#Length: %d\n", inputStr.length);
    // printf("#commnad: %s\n", inputStr.command);
    // printf("#isBackground: %d\n", inputStr.isBackground);

    // //print args
    // for(struct Link * link = inputStr.args; link != NULL; link = link->next){
    //     printf("#Arg: %s\n", link->data);
    // }
    // }


    //free the memory
    // if(inputStr.inputFile != NULL){
    //     fclose(inputStr.inputFile);
    // }
    // if(inputStr.outputFile != NULL){
    //     fclose(inputStr.outputFile);
    // }
    struct Link * link= inputStr.args;
    while (link != NULL){
        struct Link * temp = link;
        link = link->next;
        free(temp->data);
        free(temp);
    }
    //free(inputStr.args); //invalid free 
    free(inputStr.command);
    free(inputStr.input);

    if (inputStr.isExit == true)
    {
        break;
    }
    }

}