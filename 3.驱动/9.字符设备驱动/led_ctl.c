#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include "myled.h"


int main(int argc, char *argv[])
{    
    int fd;
    int cmdno;
    
    cmdno = atoi(argv[1]);
    
    fd = open("/dev/my_led0", O_RDWR);
    if(fd < 0)
    {
        printf("open led fail!\n");
        return 0;	
    }
    
    if(1 == cmdno)
        ioctl(fd, LED_ON);
    else if(2 == cmdno)
        ioctl(fd, LED_OFF);
    else
        printf("please input the correct num!\n");
        
    close(fd);
         
    return 0;	
}