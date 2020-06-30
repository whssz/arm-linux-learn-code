/**********************************
*文件名：tax.c
*创建时间：2016-9-3
*创建着：wuhao
*程序说明：个人所得税计算库
***********************************/

#include <stdio.h>

int tax(int salary, int insurance)
{
   int tax_salary = salary - insurance;
   
   int tmp = tax_salary - 1500;
   
   if(tmp < 0)
   {
      printf("you don't need tax!\n");
      return 0;
   }
   
   if(tmp < 1500)
      return tmp*0.03-0;
      
   if( (tmp >= 1500) && (tmp < 4500) )
      return tmp*0.1-105;
      
   if( (tmp >= 4500) && (tmp < 9000) )
      return tmp*0.2-555;
      
   if( (tmp >= 9000) && (tmp < 35000) )
      return tmp*0.25-1005;
        
   if( (tmp >= 35000) && (tmp < 55000) )
      return tmp*0.3-2755;  
        
}









