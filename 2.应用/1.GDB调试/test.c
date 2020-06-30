#include <stdio.h>

void myprintf(int number)
{
  printf("%d\n", number);
}
int main(void)
{
  int test =0;
  unsigned char i =0;
  
  for(i=0;i<100;i++)
  {
      test++;
  }
  
  myprintf(test);
  
  return 0;
}
