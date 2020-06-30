#include  <stdio.h>

int main(void)
{ 
    int i=-1;
    if(i)
        printf("yes!\n");
        
     
     goto err1;
     err1:
         printf("1\n");
         return 0;
     err2:
         printf("2\n");
     err3:
         printf("3\n");
    return 0;
}


