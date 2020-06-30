#include <stdio.h>
#include <stdlib.h>

int b = 0 ;
int c;
static int d =0;
const int e = 1;
char *q = "abcde";

int main(void)
{ 
 int a;
 static int f;
 const int g =1;
 int * malloc_p = NULL;
 malloc_p = malloc(sizeof(int));
 
 printf("add = %p, num =%d\n", &e, e);
 printf("add = %p, num =%d\n", malloc_p, *malloc_p);
 printf("add = %p, num =%d\n", &a, a);
 printf("add = %p, num =%d\n", &c, c);
 printf("add = %p, num =%d\n", q, *q);

 while(1);
 
 return 0;
}
