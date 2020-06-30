#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>

int main(void)
{ 
  pid_t id;
  int i=10000;
  id = vfork();
  
  if(0 == id)
  {
      printf("PID is : %d\n", id);
      while(i--); 
      printf("chould process is end!\n");    
      _exit(0);    
  }
  else
  {
      printf("this is father process!\n");
      printf("I = %d\n", i);
      exit(0);
  }  

}
