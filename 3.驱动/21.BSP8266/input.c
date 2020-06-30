#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bsp8266_cmdlist.h"


int main(void)
{   
    int fd;
    int ret;
    int i;
    unsigned short read_size=0;
    unsigned int cmd_mun=0;
    unsigned char cmd1[100]=
    {0x41, 0x54, 0x2B, 0x43, 0x49, 0x50, 0x53, 0x54, 0x41, 0x52, 0x54, 0x3D, 0x22, 0x54,
     0x43, 0x50, 0x22, 0x2C, 0x22, 0x31, 0x38, 0x30, 0x2E, 0x38, 0x34, 0x2E, 0x33, 0x33, 
     0x2E, 0x37, 0x34, 0x22, 0x2C, 0x38, 0x30, 0x38, 0x36, 0x0D, 0x0A};   
    unsigned char buff[1024];
    unsigned char *cmd;
 
    fd = open("/dev/s3c2410_serial1", O_RDWR);
    //O_NONBLOCK非阻塞打开串口
    if(fd < 0)
    {
        printf("open the serial0 fail!\r\n");
        exit(0);	
    }
    
   // ret = write(fd, cmd1, strlen(cmd1));
    
    //sleep(2);

    
    while(1)
    {
    	ret = read(fd, buff, sizeof(buff));
        printf("%d\r\n", ret);
        printf("%s\r\n", buff);
        sleep(2);
    }
    close(fd);
    exit(0);
 }
    
    