#include <sys/ioctl.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "led.h"
#include <stdio.h>


int main(int argc, char *argv[])
{
     int fd;
     
     fd = open("/dev/myled", O_RDWR);
     
     if(argc < 2 )
     {
        printf("please enter the next para\n");	
        
        return 0;
     }
     int cmd = atoi(argv[1]);
     
     if(1 == cmd)
         ioctl(fd, LED_ON);       	
     else      
         ioctl(fd, LED_OFF);
         
         return 0;	 	              	
}