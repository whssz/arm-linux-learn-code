#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


void main()
{  
   pid_t pid;
   
   pid = vfork();
   if(pid < 0)
   perror("creat the vfok fail!\n");
   
   if(0 == pid)
   {    
       int ret;     
       ret = execl("/home/r_proess", "r_proess", NULL);  /*执行可执行文件*/
       //ret = execl("/bin/hello", "hello", NULL);
       if(ret < 0)
           perror("run the child process fail!\n");
       exit(0);
   } 
   else
   {    
        int ret; 
        sleep(1);
        ret = kill(pid, SIGINT);  /*获取子进程的pid*/
        if(ret < 0)
            perror("send the signal fail!\n");
        printf("yes!\n");
        exit(0);
   }
}
