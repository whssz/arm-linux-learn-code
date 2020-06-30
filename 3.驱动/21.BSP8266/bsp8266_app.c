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
unsigned char read_buf[1024]; /*�����ȡ������*/
unsigned char *cmd;  /*�������*/
 
int main(void)
{ 
    struct termios opt; 
    struct termios oldopt;   
    int ret =0;
    
    /*�򿪴����豸, �ɶ���д, ����Ϊ�����ж�*/
    fd = open("/dev/s3c2410_serial1", O_RDWR | O_NOCTTY );
    if(fd == -1)
    {
        perror("open serial 0\n");
        exit(0);
    }
    
    bzero( &opt, sizeof( opt ) );     /*����ն������ṹ*/
    if ( tcgetattr( fd,&opt) != 0)   
        {   
            perror("SetupSerial fail\n");  
            return -1;  
        }     

    opt.c_cflag |= (CLOCAL | CREAD); //�����
    opt.c_cflag &= ~CSIZE; //����Ϊ����
    opt.c_cflag &= ~CRTSCTS; //��ʹ��Ӳ������
    opt.c_iflag |= IGNPAR; //������żУ�����
    opt.c_cflag |= CS8; //8λ����λ
    opt.c_cflag &= ~CSTOPB;  //1λֹͣλ      
    

    cfsetispeed(&opt, B115200);/*�����������������*/
    cfsetospeed(&opt, B115200);
  
    opt.c_cc[VTIME] = 10; //10 * 0.1s
    opt.c_cc[VMIN] = 0;//���ö�ȡ����С�ַ���,�͵����ַ���ȴ���ȡʱ��
    
    tcflush(fd, TCIOFLUSH);/*ˢ�������������*/
  
    if(tcsetattr(fd, TCSANOW, &opt) != 0) /*��Ч��������,������Ч*/
    {
        perror("serial error");
        return -1;
    }
  

    cmd = malloc(100);/*��������ռ�*/
    if(!cmd)
         printf("get the cmd mem fail\r\n");
       
     /*2.�������ý���*/
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
        
    /*3.����ָ��,��ɳ�ʼ��*/ 
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
   
   /*��ʼ��������*/
   set_cmd("AT+CIPSEND\r\n", "OK"); 
   free(cmd); /*�ͷ�����ռ�*/
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

/*wifi ���������*/
void set_cmd(unsigned char *cmd, unsigned char *ack)
{
    /*2.1����Ϊstationģʽ*/   
    int ret = 0;
              
    while(!strstr(read_buf, ack)) /*�ȴ�Ӧ��ɹ�*/
    {             	        	 
        ret = write(fd, cmd, strlen(cmd)); /*д������*/
        if(ret < 0)
        printf("write fail!\n");
                                   
        sleep(2);/*��ʱһ��*/
                
        ret = read(fd, read_buf, sizeof(read_buf)); /*��ȡ����*/              
        if(ret < 0)
        printf("read fail!\n");  
          
        read_buf[strlen(read_buf)] ='\0';/*��ӽ�����*/
        
        printf("%s\r\n", read_buf); /*���Գ�ʼ����Ϣ*/
    }  
                 
    printf("the cmd config success!\r\n");  /*�ɹ�Ӧ��*/
    memset(read_buf, 0, sizeof(read_buf));  /*�����־*/ 
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