#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

int main(void)
{ 
   int fd = 0;
   char W_buf[10] = "12345";
   char R_buf[10];
   fd = open("/home/2.应用/8.系统调用文件编程/test.c", O_RDWR | O_CREAT, 0755);
   if(fd < 0)
       printf("open file fail!\n");
   
   // W_buf[5] = '\0';    /*注意加上字符串结束符*/
   //close(fd);
   int ret1 = write(fd, W_buf, 6);
   //printf（"%c\n",W_buf[5]）;
   if(ret1 < 0)     
       printf("write file fail!\n");
       
   
   
   int ls = lseek(fd, -6, SEEK_END); 
   if(ls < 0)
      printf("lseek fail!\n");
   else
   	  printf("ls is : %d\n", ls);
      
   // int fd2 = dup(fd);
      
   //strlen(W_buf)
   
         
   int ret2 = read(fd, R_buf, 3);
   R_buf[3] = '\0';
   if(ret2 < 0)
      printf("read file faile!\n");
   else 
      printf("the buf : %s\n", R_buf);  
    
   
  return 0;
}

