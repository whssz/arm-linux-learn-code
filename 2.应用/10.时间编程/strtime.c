#include <time.h>

int main(void)
{  
  time_t ctime;
  struct tm *gm;
  char *strtime;
  
  ctime = time(NULL);
  
  gm = localtime(&ctime);
  
  strtime = asctime(gm);
  
  printf("the time is %d : %d\n",gm->tm_hour, (*gm).tm_min);
  
  printf("the time is %s\n", strtime);
  
  return 0;	
}