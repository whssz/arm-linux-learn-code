#include <stdio.h>
#include <sys/syscall.h>
#include <unistd.h>

void mypk()
{
  /*__asm__ (
  "ldr r7,=365 \n"
  "swi \n"
  :
  :
  :"memory"); 
  */ 
  	
}


int main(void)
{ 
  // mypk();	
  syscall(365, 2); 
  return 0; 	
}
