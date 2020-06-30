/********************************
*文件名 : daemon.c
*创建日期 : 2016-9-14
*创建人: wuhao
*文件说明 : 创建守护进程,写入文件数据
*********************************/
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>


int main(void)
{  
  int pid;
  unsigned int i;
  char *buf = "hello world!";
  /*创建一个子进程,后台运行*/
  pid = fork();
  if(pid < 0)
  {
     printf("creat fork fail!\n");
     exit(1); //子进程异常退出.exit(1)	
  }
  else if(pid > 0)
  {
    exit(0); //父进程退出	
  }
  else 
  {
    /*独立于控制终端*/
     setsid();
    /*3 摆脱父进程的影响*/
       /*3.1 改变工作目录为根目录 */
     chdir("/");
       /*3.2 清除文件权限掩码*/
     umask(0);
       /*3.3 关闭打开文件*/
     for(i=0; i<65535; i++)
        close(i);
     while(1)
     {
        int fd = open("/home/test11111.log", O_APPEND|O_RDWR|O_CREAT, 0777);	
        if(fd < 0 )
        {
           printf("open file error\n");
           exit(1); //跳出进程	
        }
       
        write(fd, buf, strlen(buf));  
        close(fd);  
        sleep(2);
     }	
          
  }
  
	
  return 0;	
}