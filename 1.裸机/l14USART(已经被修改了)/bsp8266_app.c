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
    

    while(1)
    {   
        printf("\r\n");
        printf("Please user input the config cmd!\n");
        printf("cmd_num:\r\n");
        printf("     1. set the wifi to station mode......\r\n");
        printf("     2. restart the wifi to take effect......\r\n");
        printf("     3. take the wifi enter the house's wifi......\r\n");
        printf("     4. set the wifi to only one connet mode!\r\n");
        printf("     5. enter the wifi server!\r\n");
        printf("     6. set the wifi to the unvarnished trans mode!\r\n");
        printf("     7. start trans\r\n");
        printf("\r\n");
        
        scanf("%d", &cmd_mun);  /*输入命令*/
        
        switch(cmd_mun)
        {
            /**************************cmd:1*********************************/
            case 1:
            cmd = malloc(100);
            cmd = "AT+CWMODE=1\r\n";  /*设置为station模式*/
            
            ret = write(fd, cmd, strlen(cmd)); /*写入命令*/
            if(ret < 0)
                printf("write fail!\n");
            printf("inpout count : %d\r\n", ret);
            	
            sleep(4);
            
            //printf("1!\n");
            ret = read(fd, buff, sizeof(buff));           
            if(ret < 0)
                printf("read fail!\n");
                
            printf("output countL %d\r\n", ret);   
             /*添加结束符*/ 
            buff[strlen(buff)] ='\0';
            printf("%s\r\n", buff); 
              
             /*判断是否成功应答*/
            if(!strstr((const char*)buff, "OK"))
            {
                printf("the cmd config fail!\r\n");
                printf("please input the cmd again!\r\n");	
            }
            else
            {
                printf("the cmd config success!\r\n");
                free(cmd);	
            }
            break;
            
            /**************************cmd:2*********************************/
            case 2:
            cmd[100] = "AT+RST\r\n";
            ret = write(fd, cmd, strlen(cmd)); /*重启wifi生效*/
            if(ret < 0)
                printf("write fail!\n");
            	
            sleep(1);
        
            ret = read(fd, buff, sizeof(buff));
            if(ret < 0)
                printf("read fail!\n");
                                 
            /*判断是否成功应答*/
            if(!strstr(buff, "OK"))
            {
                printf("the cmd config fail!\r\n");
                printf("please input the cmd again!\r\n");	
            }
            else
            {
                printf("the cmd config success!\r\n");	
            }    
            break;
            
            /*case 3:
            ret = write();
            if(ret < 0)
                printf("write fail!\n");
            	
            msllep(20);
        
            ret = read()
            if(ret < 0)
                printf("read fail!\n");
            break;
            
            case 4:
            ret = write();
            if(ret < 0)
                printf("write fail!\n");
            	
            msllep(20);
        
            ret = read()
            if(ret < 0)
                printf("read fail!\n");
            break;
            */
            default:
            printf("please input the right config cmd\r\n");     
       }
    }
    close(fd);
    exit(0);	
}