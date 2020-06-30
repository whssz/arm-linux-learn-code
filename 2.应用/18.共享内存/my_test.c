#include <stdlib.h>
#include <string.h>


struct Test
{
    char *p;
    int  num;
};
int main(void)
{
    struct Test test; 
    
    test.p = malloc(2);
    test.num = 1;
    test.p[1] = 'w';
    test.p[2] = 'u';
    test.p[3] = 'u';
    test.p[4] = 'u';
    test.p[5] = 'u';
    test.p[6] = 'u';
    test.p[7] = 'u';
    printf("%d\n", sizeof(test));
    printf("%d\n", test.p[7]);
    
    free(test.p);
    
    char str[] = "wuhao";
    char *q = "wuhao";
    printf("%d\n", sizeof(str));
    printf("%d\n", sizeof(q));
    
    return 0;
}
