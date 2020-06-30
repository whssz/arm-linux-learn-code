#include <signal.h>
#include <stdio.h>

void my_dell()
{
  printf("get the SIGINT\n");	
}

int main(void)
{ 		
  /*设置信号处理模式*/
  signal(SIGINT, my_dell);
  /*等待接收到信号*/
  pause();	
	
  return 0;	
}