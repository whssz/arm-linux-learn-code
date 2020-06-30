#include <sys/ioctl.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "my_platform.h"
#include <stdio.h>


int main(int argc, char *argv[])
{
     int fd;
     
     fd = open("/dev/my_misc_driver", O_RDWR);
     
     if(argc < 2 )
     {
        printf("please enter the next para\n");	
        
        return 0;
     }
     int cmd = atoi(argv[1]);
     
     if(1 == cmd)
         ioctl(fd, BEEP_ON);       	
     else      
         ioctl(fd, BEEP_OFF);
         
         return 0;	 	              	
}