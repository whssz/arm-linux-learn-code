#include <time.h>

int main(void)
{  
  time_t ctime;
  struct tm *gm;
  
  ctime = time(NULL);
  
  gm = localtime(&ctime);
  
  printf("the time is %d : %d\n",gm->tm_hour, (*gm).tm_min);
  
  return 0;	
}