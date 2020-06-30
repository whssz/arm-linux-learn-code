#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void)
{ 
  int fd ;
  int number;
  
  fd = open("/dev/memdev0", O_RDONLY);
  
  read(fd, &number, sizeof(int));
  
  printf("the number is %d\n", number);
  
  return 0;	
}