#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>


int main(void)
{ 
   int fd;
  /*创建FIFO*/
  mkfifo("/home/smb/myfifo",0666);
  
  /*打开文件*/
  fd = open("/home/smb/myfifo", O_WRONLY);
  
  /*写入数据*/
  write(fd, "hello", 6);
  
  /*关闭文件*/
  close(fd); 
  
  return 0;	
}
