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
   
   /*��fifo*/
   fd = open("/home/smb/myfifo", O_RDONLY);
   
   /*��ȡ����*/
   read(fd, buf, 6);  
   
   printf("the str is : %s\n", buf);
   
   close(fd);
   
   unlink("home/smb/myfifo");
      	
   exit(0);
   
}
