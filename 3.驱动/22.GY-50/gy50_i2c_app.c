#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "gy_50.h"

unsigned char readbuf[6];
unsigned char reg_init[5][2]={  /*初始化寄存器,和值*/
    {CTRL_REG1, 0x0f},
    {CTRL_REG2, 0x00},
    {CTRL_REG3, 0x08},
    {CTRL_REG4, 0x00},
    {CTRL_REG5, 0x00},	
};

void desplay_X(void);
void desplay_Y(void);
void desplay_Z(void);


int main(void)
{  
    int fd,ret;
    unsigned char i=0;
   
    fd  = open("/dev/gy50", O_RDWR);
    if(fd < 0)
        perror("gy50 open fail\n");
      
      
      /* ret = write(fd, reg_init[i], 2);
       if(ret < 0)
       {
           perror("write reg to gy50 fail!\n");
           return -1;
       } */
    /*初始化gy50*/
    for(i=0; i<5; i++)
    {
       ret = write(fd, reg_init[i], 2);
       if(ret < 0)
       {
           perror("write reg to gy50 fail!\n");
           return -1;
       }  
       //usleep(200);     	
    }
    
    /* ret = read(fd, &readbuf[0], 1);
     if(ret < 0)
     {
        printf("read gy50 fail!\n");
     }
     printf("x :%x\n", readbuf[0]);
     */
    /*循环读取数据*/
    while(1)
    {  
    	lseek(fd, 0, SEEK_SET);
        for(i=0; i<6; i++)
        {        
           ret = read(fd, &readbuf[i], 1);
           if(ret < 0)
           {
               perror("write reg to gy50 fail!\n");
               return -1;
           }   	
        }
        
        desplay_X();
        desplay_Y();
        desplay_Z();
        printf("\n");
        printf("\n"); 
        usleep(400000);       
   }
   
   exit(0);
}


void desplay_X(void)
{
    float temp=0;
    short dis_data=0;
    dis_data =(short)((readbuf[1]<<8) | readbuf[0]);   
    temp=(float)dis_data*(0.00875); 
    dis_data = (short)(temp); 
    printf("the X: %d\n", dis_data);    	
}

void desplay_Y(void)
{
    float temp=0;
    short dis_data=0;
    dis_data =(short)((readbuf[3]<<8) | readbuf[2]);   
    temp=(float)dis_data*(0.00875); 
    dis_data = (short)(temp); 
    printf("the Y: %d\n", dis_data);    	
}

void desplay_Z(void)
{
    float temp=0;
    short dis_data=0;
    dis_data =(short)((readbuf[5]<<8) | readbuf[4]);   
    temp=(float)dis_data*(0.00875); 
    dis_data = (short)(temp); 
    printf("the Z: %d\n", dis_data);    	
}