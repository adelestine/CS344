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
    char *args[] = {"test", "-f", "badfile", NULL};



    if (childID == 0)
    {
        printf("I am the child\n"); //RUNS ONCE
        //WANT
        if(execvp("test", args) == -1)
        {
            printf("Error");
        }
        exit(1);
    }
    else
    {
        printf("I am the parent\n"); //RUNS ONCE
        waitpid(-1, &stat, 0); //wait for child to finish

    }
    if( WIFEXITED(stat) )
    {
        //printf("Child exited with code %d\n", WEXITSTATUS(stat));
    }else if( WIFSIGNALED(stat) )
    {
        //printf("Child was terminated by signal %d\n", WTERMSIG(stat));
    }



    char *args[] = {"status", "&", NULL};

    
    //printf("Only run by the parent\n"); //RUNS ONCE
    return 0;
    

}
