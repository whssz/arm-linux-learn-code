#include <sys/time.h>
#include <stdio.h>

int main(void)
{ 
  time_t ctime;
  struct timeval tv1;
  
  ctime = time(NULL);
  
  gettimeofday(&tv1,NULL);
  
  printf("the time is %d sec, %d msec\n", tv1.tv_sec,tv1.tv_usec);
  	
  return 0;	
}