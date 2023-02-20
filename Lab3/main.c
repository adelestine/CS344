#include "header.h"

/*
 *  This function gets user input and prints it to the screen
 *  @return 0
 *  
*/
int getUserInput(struct inputStr * inputStr){
    printf(": ");
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
    printf("%d", numArgs);
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
    char * inputFileName = calloc(inputStr->length, sizeof(char));      // allocate memory for the input file name
    for (j; j < inputStr->length; j++){                                 // loop through the input
        if (inputStr->input[j-1] == '<' && inputStr->input[j] == ' '){  // check if the previous character is < and the current character is a space
            j = j + 1;
            int k = 0;
            for (j; j < inputStr->length; j++){                         // loop through the name of the file
                if (inputStr->input[j] == ' ' || inputStr->input[j] == '\n'){ // check if the current character is a space or a new line and if so end reading file name
                    break;
                }
                else{
                    inputFileName[k] = inputStr->input[j];              // add the current character to the file name
                    k = k + 1;
                }
            }
            inputStr->inputFile = fopen(inputFileName, "rw"); 
            free(inputFileName);          // open the file
            break;
        }
        else if (inputStr->input[j-1] == '<' && inputStr->input[j] != ' '){ // check if the previous character is < and the current character is not a space
            printf("Error: No space after <");                              // print error message
        }
    }
    if (inputStr->inputFile == NULL)        // check if the file was opened successfully
    {
        perror("Error opening file");                   // print error message
        return(0);
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
    char * outputFileName = calloc(inputStr->length, sizeof(char));      // allocate memory for the input file name
    for (j; j < inputStr->length; j++){                                 // loop through the input
        if (inputStr->input[j-1] == '>' && inputStr->input[j] == ' '){  // check if the previous character is < and the current character is a space
            j = j + 1;
            int k = 0;
            for (j; j < inputStr->length; j++){                         // loop through the name of the file
                if (inputStr->input[j] == ' ' || inputStr->input[j] == '\n'){ // check if the current character is a space or a new line and if so end reading file name
                    break;
                }
                else{
                    outputFileName[k] = inputStr->input[j];              // add the current character to the file name
                    k = k + 1;
                }
            }
            inputStr->outputFile = fopen(outputFileName, "rw"); 
            free(outputFileName);          // open the file
            break;
        }
        else if (inputStr->input[j-1] == '>' && inputStr->input[j] != ' '){ // check if the previous character is < and the current character is not a space
            printf("Error: No space after <");                              // print error message
        }
    }
    if (inputStr->outputFile == NULL)        // check if the file was opened successfully
    {
        perror("Error opening file");                   // print error message
        return(0);
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
    i = 0  ;
    //read Command

    while (i < inputStr->length && inputStr->input[i] != ' '){
        //set the command
        inputStr->command[i] = inputStr->input[i];
        i = i + 1;
    }
    //printf("Command: %s\n", inputStr->command);
    //read args
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


    //remove first Arg
    // struct Link * temp = inputStr->args;
    // inputStr->args = inputStr->args->next;
    // free(temp);



}


/*
           ___
     _..--"\  `|`""--.._
  .-'       \  |        `'-.
 /           \_|___...----'`\
|__,,..--""``(_)--..__      |
'\     _.--'`.I._     ''--..'
  `''"`,#JGS/_|_\###,.--'`
    ,#'  _.:`___`:-._'#,
   #'  ,~'-;(oIo);-'~, '#
   #   `~-(  |    )=~`  #
   #       | |_  |      #
   #       ; ._. ;      #
   #  _..-;|\ - /|;-._  #
   #-'   /_ \\_// _\  '-#
 /`#    ; /__\-'__\;    #`\
;  #\.--|  |O  O   |'-./#  ;
|__#/   \ _;O__O___/   \#__|
 | #\    [I_[_]__I]    /# |
 \_(#   ;  |O  O   ;   #)_/
        |  |       |
        |  ;       |
        |  |       |
        ;  |       |
        |  |       |
        |  |       ;
        |  |       |
        '-.;____..-'
          |  ||  |
          |__||__|
          [__][__]
        .-'-.||.-'-.
       (___.'  '.___)


*/

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
    if (strcmp(inputStr->command, "cd") == 0){
        //change directory
        //printf("cd\n");
        //printf("Arg: %s\n", inputStr->args->data);
        if (inputStr->args->data == NULL){
            chdir(getenv("HOME"));
        }else{
            chdir(inputStr->args->data);
        }
        return 1;
    }else if (strcmp(inputStr->command, "exit") == 0){
        //exit
        //printf("exit\n");
        exit(0);
        return 1;
    }else if (strcmp(inputStr->command, "status") == 0){
        //status
        //printf("status\n");
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
    printf("->runCommand\n");
    printf("->Command: %s\n", inputStr->command);
    //printf("Arg: %s\n", inputStr->args->data);
    //printf("Arg: %s\n", inputStr->args->next->data);
    //printf("Arg: %s\n", inputStr->args->next->next->data);
    //printf("Arg: %s\n", inputStr->args->next->next->next->data);

    //char * args[inputStr->argCount];
    int stat;
    char * temp = inputStr->command;
    
    
    printf("temp: %s\n", temp);




    char *args[] = {"ls", NULL};
    char *args2[inputStr->argCount];
    //copy args to static array
    int i = 0;

    struct Link * link = inputStr->args;
    while (link != NULL){
        args2[i] = link->data;
        link = link->next;
        i = i + 1;
    }
    args2[i] = NULL;
    //print args
    for (int i = 0; i < 3; i++){
        printf("->Arg0%d: %s\n",i, args[i]);
        printf("->Arg2%d: %s\n",i, args2[i]);
    }
    int childID = fork();
    // //fork
    // int stat;
    // pid_t spawnPid = fork();

    if (childID == 0)
    {
        printf("=>Child\n");
        //printf("=>Child PID: %d\n", getpid());
        //printf("=> inputStr->command: %s\n", inputStr->command);
        //printf("=> args[0]: %s\n", args[0]);
        if (execvp(args[0], args) == -1)
        {
            printf("=>Child: This should not be seen\n");
            perror("execvp");
            exit(1);
        }


    }else{
        printf("-->Parent\n");
        // printf("-->Parent PID: %d\n", getpid());
        // printf("-->Child PID: %d\n", spawnPid);
        waitpid(-1, &stat, 0);
    }
    if ( WIFEXITED(stat) )
    {
        printf("-->exit value %d\n", WEXITSTATUS(stat));
    }else if (WIFSIGNALED(stat)){
        printf("-->terminated by signal %d\n", WTERMSIG(stat));
    }

    
    
    






    
    //printf("->spawnPid: %d\n", spawnPid);

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
    

    getUserInput(&inputStr);
    parseInput(&inputStr);
    runCommand(&inputStr);
    
    

    //print Input 




    //testing

    //only print if parent process
    if(getpid() == 0){
    
    printf("#Input: %s\n", inputStr.input);
    printf("#Length: %d\n", inputStr.length);
    printf("#commnad: %s\n", inputStr.command);
    printf("#isBackground: %d\n", inputStr.isBackground);

    //print args
    for(struct Link * link = inputStr.args; link != NULL; link = link->next){
        printf("#Arg: %s\n", link->data);
    }
    }


    //free the memory
    if(inputStr.inputFile != NULL){
        fclose(inputStr.inputFile);
    }
    if(inputStr.outputFile != NULL){
        fclose(inputStr.outputFile);
    }
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

}