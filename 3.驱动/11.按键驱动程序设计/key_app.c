#include <stdio.h>
#include <stdlib.h>

int main(void)
{    
    int fd;
    int key_num;
    
    /*���豸*/
    fd = open("/dev/my_key", 0);
    if(fd < 0)
        printf("open device fail!\n");
        
    /*��ȡ�豸*/
    read(fd, &key_num, 4);
    printf("key is %d\n", key_num);
    
    /*�ر��豸*/
    close(fd);
}