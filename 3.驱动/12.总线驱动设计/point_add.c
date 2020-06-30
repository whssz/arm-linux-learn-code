#include  <stdio.h>

#define ADDR1   0x10000000
#define ADDR2  (unsigned long *)0x10000000

int main(void)
{   
    printf("ADDR1 = %x\n", ADDR1+1);
    printf("ADDR2 = %X\n", ADDR2+1);
    
    return 0;
}
