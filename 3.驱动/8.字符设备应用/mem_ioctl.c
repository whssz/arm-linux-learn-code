#include "memdev.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdio.h>


int main(void)
{   
    int fd;
    fd = open("/dev/memdev0", O_RDWR);  
    if(fd < 0)
    {
        printf("open the dev fail!\n");
        return 0;	
    }
    
    ioctl(fd, MEM_SET, 115200);
    ioctl(fd, MEM_RESTART);
    
    return 0;	
}