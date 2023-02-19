#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>


int main()
{
    
    printf("Hello World!\n"); //RUNS ONCE
    int stat;
    int childID = fork();     //TECHNICALLY RUNS TWICE

    printf("my PID is %d\n", getpid());         //RUNS TWICE
    printf("my child's PID is %d\n", childID);  //RUNS TWICE
    char *args[] = {"ls", "-l", NULL};



    if (childID == 0)
    {
        printf("I am the child\n"); //RUNS ONCE
        //WANT
        if(execvp("ls", args) == -1)
        {
            printf("Error");
        }
        exit(1);
    }
    else
    {
        printf("I am the parent\n"); //RUNS ONCE
        wait(&stat); //wait for child to finish
    }
    if( WIFEXITED(stat) )
    {
        printf("Child exited with code %d\n", WEXITSTATUS(stat));
    }else if( WIFSIGNALED(stat) )
    {
        printf("Child was terminated by signal %d\n", WTERMSIG(stat));
    }
    
    printf("Only run by the parent\n"); //RUNS ONCE
    return 0;
    

}
