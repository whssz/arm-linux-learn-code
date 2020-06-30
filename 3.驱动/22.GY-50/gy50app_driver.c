#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/types.h>
#include <sys/ioctl.h>
#include <string.h>
#include <stdlib.h>
#include "gy_50.h"

int fd;
struct i2c_rdwr_ioctl_data gy50_data; /*定义消息结构体*/
unsigned char x_y_z[6];

int main(void)
{     
   
    char temperature=0;
    /*1.打开i2c设备*/
    fd = open("/dev/i2c-0", O_RDWR);
    if(fd < 0)
       printf("open the i2c_bus fail!\n");
       
    /*分配i2c_msg*/    
    gy50_data.msgs = (struct i2c_msg *)malloc(2*sizeof(struct i2c_msg));
    if(NULL ==  gy50_data.msgs)
        printf("get the msgsbuf fail!\n");
   

    /*2.初始化L3G4200*/
    Single_WriteL3G4200D(CTRL_REG1, 0x0f); /*使能输出*/
    usleep(200);  
    Single_WriteL3G4200D(CTRL_REG2, 0x00);   
    usleep(200); 
    Single_WriteL3G4200D(CTRL_REG3, 0x08); 
    usleep(200);   
    Single_WriteL3G4200D(CTRL_REG4, 0x00);   /*250*/
    usleep(200); 
    Single_WriteL3G4200D(CTRL_REG5, 0x00);
    usleep(200); 
    
   
    /*3. 读取温度*/
   
    
  
    while(1)
    {   
       temperature = Single_ReadL3G4200D(OUT_TEMP);
       printf("the temperature is :%d\n", temperature);
       
       X_Y_Z_display();
       memset(x_y_z, 0, 6);
       
       usleep(400000);
    }
    
    /*4.释放资源,关闭设备*/
    free(gy50_data.msgs);
    close(fd);  
    exit(0);	
}
   
/*往l3g写入一个字节*/
void Single_WriteL3G4200D(unsigned char REG_Address,unsigned char REG_data)
{
    /*1. 构建消息*/
    gy50_data.nmsgs = 1;   //写操作
    gy50_data.msgs[0].len = 2;  //写入数据的字节长度,
    gy50_data.msgs[0].flags = 0;  //写方向
    gy50_data.msgs[0].addr = SlaveAddress; //设备地址 
    gy50_data.msgs[0].buf = (unsigned char *)malloc(2);  //为buf申请空间
    if(NULL == gy50_data.msgs[0].buf)
        printf("get the senfbuf fail!\n"); 
    gy50_data.msgs[0].buf[0] = REG_Address;  //写入字节地址
    gy50_data.msgs[0].buf[1] = REG_data;  //写入的数据
    
    /*2.l3g4200发送消息*/
    ioctl(fd, I2C_RDWR, (unsigned long)&gy50_data);    	
}

/*往l3g读出一个字节*/
unsigned char Single_ReadL3G4200D(unsigned char REG_Address)
{      
    unsigned char read_data=0;                    
    /*1.写入从机地址和寄存器地址*/   
    gy50_data.nmsgs = 2;   //消息个数
    gy50_data.msgs[0].len = 1;  //字节长度,不包括从机地址,因为初始化已经写于从机地址
    gy50_data.msgs[0].flags = 0;  //写方向
    gy50_data.msgs[0].addr = SlaveAddress; //从机地址
    gy50_data.msgs[0].buf[0] = REG_Address;  //写入字节地址
      
    gy50_data.msgs[1].len = 1;  //字节长度,不包括从机地址,因为初始化已经写于从机地址
    gy50_data.msgs[1].flags = 1;  //读方向
    gy50_data.msgs[1].addr = SlaveAddress; //从机地址
    gy50_data.msgs[1].buf = (unsigned char *)malloc(2);  //为buf申请空间
    if(NULL ==  gy50_data.msgs[1].buf )
        printf("get the recievebuf fail!\n");
    gy50_data.msgs[1].buf[0] = 1;  //接收到的数据存放
  
    /*2.从gy50_data中读出数据*/
    ioctl(fd, I2C_RDWR, (unsigned long)&gy50_data);       
    read_data =  gy50_data.msgs[1].buf[0]; 
    free(gy50_data.msgs[1].buf);
    
    return read_data; 
}

unsigned char X_Y_Z_display(void)
{  
    float temp=0;
    short dis_data=0;
    
    /*1. 读取x轴的角加速度*/
    x_y_z[0] = Single_ReadL3G4200D(OUT_X_L);     
    x_y_z[1] = Single_ReadL3G4200D(OUT_X_H);
 
    dis_data=(short)((x_y_z[1]<<8) + x_y_z[0]);       //合成数据   
    
    temp=(float)dis_data*(0.00875);         // 数据手册 第9页，2000度/秒量程
    dis_data=(short)temp;
        printf("the X: %d\n", dis_data); 
        
    dis_data=0;
    temp =0;
    
    /*2. 读取y轴的角加速度*/
    x_y_z[2] = Single_ReadL3G4200D(OUT_Y_L);     
    x_y_z[3] = Single_ReadL3G4200D(OUT_Y_H);
   
    dis_data=(short)((x_y_z[3]<<8) + x_y_z[2]);       //合成数据   
    
    temp=(float)dis_data*(0.00875);         // 数据手册 第9页，2000度/秒量程
    dis_data=(short)temp;
        printf("the Y: %d\n", dis_data); 
        
    dis_data=0;
    temp =0;
 
    /*3. 读取z轴的角加速度*/
    x_y_z[4] = Single_ReadL3G4200D(OUT_Z_L);     
    x_y_z[5] = Single_ReadL3G4200D(OUT_Z_H);
  
    dis_data=(short)((x_y_z[5]<<8) + x_y_z[4]);       //合成数据   
    
    temp=(float)dis_data*(0.00875);         // 数据手册 第9页，2000度/秒量程
    dis_data=(short)temp;
        printf("the Z: %d\n", dis_data); 
        
    dis_data=0;
    temp =0;
    
    return 0;	
}