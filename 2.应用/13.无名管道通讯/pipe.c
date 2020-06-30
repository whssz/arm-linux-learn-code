#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>


int main(void)
{ 
  pid_t id;
  int pipefd[2];
  char buf[10];
  
  /*常见管道*/
  pipe(pipefd);
  /*创建子进程*/
  id = fork();
  
  if(id > 0)
  {
    write(pipefd[1], "hello", 6);
    close(pipefd[1]);
    /*等待子进程*/
    wait(NULL);
    exit(0);	
  }
  
  if(id == 0)
  {
    read(pipefd[0], buf, 6);
     close(pipefd[0]);
    printf("the str is %s\n", buf);
    exit(0);
  }
	
  return 0;	
}