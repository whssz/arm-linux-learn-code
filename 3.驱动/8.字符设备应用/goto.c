#include <stdio.h>

int main(void)
{
    goto err1;
    printf("yes0\n");
    
    err1: 
        printf("yes1\n");     
    err2: 
        printf("yes2\n");   
    return 0; 
}
