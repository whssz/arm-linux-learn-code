/********************************
*�ļ��� : daemon.c
*�������� : 2016-9-14
*������: wuhao
*�ļ�˵�� : �����ػ�����,д���ļ�����
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
  /*����һ���ӽ���,��̨����*/
  pid = fork();
  if(pid < 0)
  {
     printf("creat fork fail!\n");
     exit(1); //�ӽ����쳣�˳�.exit(1)	
  }
  else if(pid > 0)
  {
    exit(0); //�������˳�	
  }
  else 
  {
    /*�����ڿ����ն�*/
     setsid();
    /*3 ���Ѹ����̵�Ӱ��*/
       /*3.1 �ı乤��Ŀ¼Ϊ��Ŀ¼ */
     chdir("/");
       /*3.2 ����ļ�Ȩ������*/
     umask(0);
       /*3.3 �رմ��ļ�*/
     for(i=0; i<65535; i++)
        close(i);
     while(1)
     {
        int fd = open("/home/test11111.log", O_APPEND|O_RDWR|O_CREAT, 0777);	
        if(fd < 0 )
        {
           printf("open file error\n");
           exit(1); //��������	
        }
       
        write(fd, buf, strlen(buf));  
        close(fd);  
        sleep(2);
     }	
          
  }
  
	
  return 0;	
}