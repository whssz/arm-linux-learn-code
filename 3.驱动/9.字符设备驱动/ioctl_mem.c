#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "memdev.h"


int main(void)
{ 
    int fd ;
     
    fd = open("/dev/memdev0", O_RDWR);
    
    ioctl(fd, MEM_SET, 115200); 
    ioctl(fd, MEM_RESET);  
    return 0;	
}