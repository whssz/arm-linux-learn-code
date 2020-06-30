/*******************************
*函数名： mytax（）
*创建时间： 2016-9-3
*创建人： wuhao
*函数功能说明： 计算个人所得税
********************************/

#include <stdio.h>
#include "tax.h"

int main(void)
{ 
  int my_tax = 0;
  
  my_tax = tax(5000, 300);
  
  printf("my tax ： %d\n", my_tax);
  
  return 0;
}
