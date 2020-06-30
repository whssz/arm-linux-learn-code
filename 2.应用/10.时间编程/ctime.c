#include <time.h>

int main(void)
{  
  time_t ctime;
  
  ctime = time(NULL);
  
  printf("the ctime is: %d\n", ctime);
  
  return 0;	
}