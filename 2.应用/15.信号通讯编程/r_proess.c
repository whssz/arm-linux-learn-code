#include <signal.h>
#include <stdio.h>

void my_dell()
{
  printf("get the SIGINT\n");	
}

int main(void)
{ 		
  /*�����źŴ���ģʽ*/
  signal(SIGINT, my_dell);
  /*�ȴ����յ��ź�*/
  pause();	
	
  return 0;	
}