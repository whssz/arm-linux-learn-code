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
  
  /*�����ܵ�*/
  pipe(pipefd);
  /*�����ӽ���*/
  id = fork();
  
  if(id > 0)
  {
    write(pipefd[1], "hello", 6);
    close(pipefd[1]);
    /*�ȴ��ӽ���*/
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