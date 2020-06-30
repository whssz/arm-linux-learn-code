#include <stdio.h>

int main(void)
{   
    char *array;
    int  i=0; 
    
    array = malloc(10);
    if(array == NULL)
        printf("get the array is fial!\n");
        
    for(i=0; i<5; i++)
    {
        array[i] = i;
        printf("the num is %d\n", array[i]);
     }   
    return 0;
}
