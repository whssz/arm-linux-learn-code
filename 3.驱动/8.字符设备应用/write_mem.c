#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(void)
{ 
  int fd ;
  char number[10] = {1,2};
  
  fd = open("/dev/memdev0", O_WRONLY);
  
  write(fd, number, 2);
  
  close(fd);
  
  return 0;	
}