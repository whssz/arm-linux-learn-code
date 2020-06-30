#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void)
{ 
  int fd ;
  char number[10] ;
  unsigned int i = 0;
  
  fd = open("/dev/memdev0", O_RDONLY); 
  if(fd < 0)
      printf("open file fail!\n");
      
  int ret2 = lseek(fd, 0, SEEK_SET); 
   printf("the f_pos is %d\n", ret2);
  
  int ret = read(fd, number, 2);
  if(ret < 0)
      printf("read fail!\n");
   
  printf("the count is %d\n", ret);
  
  printf("the data is : \r\n"); 
  
  for(i=0; i<ret; i++)
      printf("%d\r\n", number[i]);
  
  
  close(fd);
  
  return 0;	
}
