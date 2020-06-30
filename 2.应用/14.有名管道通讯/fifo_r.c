#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{ 
   int fd;
   char buf[10];
   
   /*打开fifo*/
   fd = open("/home/smb/myfifo", O_RDONLY);
   
   /*读取数据*/
   read(fd, buf, 6);  
   
   printf("the str is : %s\n", buf);
   
   close(fd);
   
   unlink("home/smb/myfifo");
      	
   exit(0);
   
}
