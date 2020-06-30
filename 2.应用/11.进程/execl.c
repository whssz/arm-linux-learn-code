#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>


int main(void)
{ 
  pid_t id;
  
  id = fork();
  
  //wait(NULL);
   
  if(id ==0)
  {
     execl("/bin/cat", "cat", "/home/uImage",  NULL);
     exit(0);  	
  }
  else
  {
     printf("%d", id);
     exit(0);	
  }      	
}
