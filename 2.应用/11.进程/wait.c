#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(void)
{
    pid_t pid1,pid2;
    
    pid1 = fork();
   
    if(pid1 == 0)
    {
        printf("1\n");
    }
    else
    {  
       pid2 = fork();
       if(pid2 == 0)
       {
           sleep(1);
           printf("2\n");
       }
       else
       {
            wait(NULL);
            printf("3\n");
       }  
    }
    exit(0);
}
