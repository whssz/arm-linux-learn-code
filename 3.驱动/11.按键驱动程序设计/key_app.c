#include <stdio.h>
#include <stdlib.h>

int main(void)
{    
    int fd;
    int key_num;
    
    /*打开设备*/
    fd = open("/dev/my_key", 0);
    if(fd < 0)
        printf("open device fail!\n");
        
    /*读取设备*/
    read(fd, &key_num, 4);
    printf("key is %d\n", key_num);
    
    /*关闭设备*/
    close(fd);
}