#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(void)
{ 
  int fd ;
  int number = 1024;
  
  fd = open("/dev/memdev0", O_WRONLY);
  
  write(fd, &number, sizeof(int));
  
  return 0;	
}