#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include "humen_check.h"

int main(void)
{
    int fd;
    char beep_flag=0;
    int ret=0;
    fd = open("dev/humen_check", O_RDWR);
    if(fd < 0)
    {
       printf("open the humen_check fail!\n");
       return 0;
    }   
    
    while(1)
    {
        ret = read(fd, &beep_flag, 1);
        if(ret < 0)
        { 
            printf("read fail!  read again!.....\n");
            continue; 
        
        }          
        if(beep_flag)
        {
            ioctl(fd, BEEP_ON);
            sleep(2); /*·äÃùÆ÷1s*/
            ioctl(fd, BEEP_OFF); /*¹Ø·äÃùÆ÷*/
            beep_flag = 0; /*±êÖ¾ÖÃ0*/
            write(fd, &beep_flag,1);	          
        }           
    }
    close(fd);
    
    return 0;
}
