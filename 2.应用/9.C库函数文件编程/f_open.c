/************************************
* �ļ���: f_open.c
* ��������: 2016-9-4
* ������: wuhao
* ��������: C�⺯���ļ����
*************************************/

#include <stdio.h>

int main(void)
{
  FILE * fd;
  char w_buf[10] = "12345";
  char r_buf[10];
  
  fd = fopen("/home/test.txt", "r+");
  if(fd == NULL)
     printf("open file fail!\n");
  
  fwrite(w_buf, 1, 5, fd);
  
  fseek(fd, 0, SEEK_SET);
  
  fread(r_buf, 1, 5, fd);
  r_buf[5] = '\0';
  printf("the str is : %s\n", r_buf);
  
  fclose(fd);  
  
  return 0;	
}