#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

int glovar = 7;
int main(void)
{ 
  pid_t id;
  
  id = fork();
  if(id < 0)
      perror("fork error!\n");
  if(0 == id)
  {
      glovar++;
      
  }
  else
  {
      glovar++;
  }
     
  printf("the glovar : %d\n", glovar);
  glovar++;
  printf("the glovar2 : %d\n", glovar);
  
  exit(0); 
}
