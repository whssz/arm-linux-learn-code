#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>


int main(void)
{ 
   int fd;
  /*����FIFO*/
  mkfifo("/home/smb/myfifo",0666);
  
  /*���ļ�*/
  fd = open("/home/smb/myfifo", O_WRONLY);
  
  /*д������*/
  write(fd, "hello", 6);
  
  /*�ر��ļ�*/
  close(fd); 
  
  return 0;	
}
