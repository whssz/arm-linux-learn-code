/**********************************************
*测试结果:
      1.fork创建子进程， 父子进程谁先运行不一定。
      2.从调用fork开始， 后面的程序都会被父子进程调用
      3.所以使用 if（0 == foek） 来执行子进程的部分，
        else() 执行父进程的部分， 剩下的就是 公共执行的部分。
      4. 可以使用 while（wait（NULL）！= -1） 是父进程等待所有进程推出。
**********************************************/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int x;  
 
void func1(void)
{
    printf("x = %d\n", x);
}


int main(void)
{
    pid_t pid1, pid2;
    int i = 2400;
    int pipefd[2];
    unsigned char readbuf[10];
    unsigned char y = 50;
    float z = 1.1;

    //printf("1\n");
    pipe(pipefd);
    
    pid1 = fork();
    
    if(0 ==pid1)
    {    
        unsigned char str[10];
        i++;
        printf("i1 = %d\n", i);
        sprintf(str, "%f\n", z);     
        write(pipefd[1], str, strlen(str));
        //close(pipefd[1]);
        //func1(); 
    }      
    else
    {  
        pid2 = fork();
        if(0 == pid2)
        {
           //sleep(1);
           read(pipefd[0], readbuf, sizeof(readbuf));                 
           printf("the readbuf is %s\n", readbuf); 
           //close(pipefd[0]);        
        }
        //printf("3\n");
        else
        {
            while(wait(NULL)!=-1);
            i++;
            printf("i2 = %d\n", i);     
            //read(pipefd[0], readbuf, sizeof(readbuf));
            //printf("the readbuf is %s\n", readbuf);
            //close(pipefd[0]);        
        }
    }   
   // printf("4\n");
   exit(0);
}
