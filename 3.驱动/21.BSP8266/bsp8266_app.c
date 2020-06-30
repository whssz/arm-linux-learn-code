#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>  
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <termios.h>
#include <errno.h>
#include <string.h>
#include "bsp8266_app.h"

int fd;
unsigned char read_buf[1024]; /*定义读取缓存区*/
unsigned char *cmd;  /*命令缓存区*/
 
int main(void)
{ 
    struct termios opt; 
    struct termios oldopt;   
    int ret =0;
    
    /*打开串口设备, 可读可写, 不作为控制中断*/
    fd = open("/dev/s3c2410_serial1", O_RDWR | O_NOCTTY );
    if(fd == -1)
    {
        perror("open serial 0\n");
        exit(0);
    }
    
    bzero( &opt, sizeof( opt ) );     /*清空终端描述结构*/
    if ( tcgetattr( fd,&opt) != 0)   
        {   
            perror("SetupSerial fail\n");  
            return -1;  
        }     

    opt.c_cflag |= (CLOCAL | CREAD); //激活串口
    opt.c_cflag &= ~CSIZE; //设置为掩码
    opt.c_cflag &= ~CRTSCTS; //不使能硬件流控
    opt.c_iflag |= IGNPAR; //忽略奇偶校验错误
    opt.c_cflag |= CS8; //8位数据位
    opt.c_cflag &= ~CSTOPB;  //1位停止位      
    

    cfsetispeed(&opt, B115200);/*设置输入输出波特率*/
    cfsetospeed(&opt, B115200);
  
    opt.c_cc[VTIME] = 10; //10 * 0.1s
    opt.c_cc[VMIN] = 0;//设置读取的最小字符数,和单个字符最长等待读取时间
    
    tcflush(fd, TCIOFLUSH);/*刷新输入输出队列*/
  
    if(tcsetattr(fd, TCSANOW, &opt) != 0) /*生效串口配置,立即生效*/
    {
        perror("serial error");
        return -1;
    }
  

    cmd = malloc(100);/*申请命令空间*/
    if(!cmd)
         printf("get the cmd mem fail\r\n");
       
     /*2.进入设置界面*/
    printf("\r\n");
    printf("***************************************************************\r\n");
    printf("The steps of init the BSP8266 :\n");   
    printf("     1. set the wifi to station mode......\r\n");
    printf("     2. restart the wifi to take effect......\r\n");
    printf("     3. take the wifi enter the house's wifi......\r\n");
    printf("     4. set the wifi to only one connet mode!\r\n");
    printf("     5. enter the wifi server!\r\n");
    printf("     6. set the wifi to the unvarnished trans mode!\r\n");
    printf("     7. start trans\r\n");
    printf("\r\n");
    printf("The WIFI is initting.................................\r\n");
    printf(".....................................................\r\n");
    printf("****************************************************************\r\n");
        
    /*3.发送指令,完成初始化*/ 
   set_cmd("AT+CWMODE=1\r\n", "OK");
   sleep(2); 
   set_cmd("AT+RST\r\n", "ready"); 
   sleep(5); 
   set_cmd(wifi_name_cmd, "OK");  
   sleep(4);
   set_cmd("AT+CIPMUX=0\r\n", "OK");    
   set_cmd(connect_cmd, "CONNECT");   
   sleep(2);    
   set_cmd("AT+CIPMODE=1\r\n", "OK"); 
   sleep(2); 
   
   /*开始发送数据*/
   set_cmd("AT+CIPSEND\r\n", "OK"); 
   free(cmd); /*释放命令空间*/
   while(1)
   { 
      ret = send_data("hello!\r\n");  
      if(ret < 0)        
          printf("send data fail!\r\n");
      sleep(2);                  
   }  
   	    
   close(fd);
   exit(0);	
}

/*wifi 发送命令函数*/
void set_cmd(unsigned char *cmd, unsigned char *ack)
{
    /*2.1设置为station模式*/   
    int ret = 0;
              
    while(!strstr(read_buf, ack)) /*等待应答成功*/
    {             	        	 
        ret = write(fd, cmd, strlen(cmd)); /*写入命令*/
        if(ret < 0)
        printf("write fail!\n");
                                   
        sleep(2);/*延时一秒*/
                
        ret = read(fd, read_buf, sizeof(read_buf)); /*读取命令*/              
        if(ret < 0)
        printf("read fail!\n");  
          
        read_buf[strlen(read_buf)] ='\0';/*添加结束符*/
        
        printf("%s\r\n", read_buf); /*回显初始化信息*/
    }  
                 
    printf("the cmd config success!\r\n");  /*成功应答*/
    memset(read_buf, 0, sizeof(read_buf));  /*清除标志*/ 
    sleep(1);	
}


int send_data(unsigned char *data)
{ 
    int ret = 0 ;
    
    ret = write(fd, data , sizeof(data)); 
    if(ret < 0)
        perror(" send data fail!\r\n");
    
    else
        printf("send %d byte success!\r\n", ret);
    
    return ret;  	
}