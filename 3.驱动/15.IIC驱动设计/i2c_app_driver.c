#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/types.h>
#include <sys/ioctl.h>
#include  <stdlib.h>



#define   SlaveAddress  0x50
#define   I2C_RDWR   0x0707
void Single_WriteL3G4200D(unsigned char REG_Address,unsigned char REG_data);
unsigned char Single_ReadL3G4200D(unsigned char REG_Address);
struct i2c_msg {
	unsigned short addr;	/* slave address			*/
	unsigned short flags;   /*读写方向,0写1读*/
	unsigned short len;		/* msg length				*/
	unsigned char *buf;		/* pointer to msg data			*/
};

 
struct i2c_rdwr_ioctl_data {
	struct i2c_msg  *msgs;	/* pointers to i2c_msgs */
	unsigned int nmsgs;	/* number of i2c_msgs */
};

int fd;
   struct i2c_rdwr_ioctl_data e2prom_data;
int main(void)
{
    
    
    //1.打开总线设备
    fd = open("/dev/i2c-0", O_RDWR);
    if(fd < 0)
       printf("open the i2c_bus fail!\n");
       
    e2prom_data.msgs = (struct i2c_msg *)malloc(2*sizeof(struct i2c_msg));
    if(NULL ==  e2prom_data.msgs)
        printf("get the msgsbuf fail!\n");
   
    //2.构造写入eeprom的消息
   /* e2prom_data.nmsgs = 1;   //消息个数
    e2prom_data.msgs[0].len = 2;  //字节长度,2个字节,写入地址和数据
    e2prom_data.msgs[0].flags = 0;  //写方向
    e2prom_data.msgs[0].addr = 0x50; //设备地址
    e2prom_data.msgs[0].buf = (unsigned char *)malloc(2);  //为buf申请空间
    if(NULL == e2prom_data.msgs[0].buf)
        printf("get the senfbuf fail!\n");
    e2prom_data.msgs[0].buf[0] = 0x10;  //写入字节地址
    e2prom_data.msgs[0].buf[1] = 0x30;  //写入的数据
    
    //3.向eeprom发送消息
    ioctl(fd, I2C_RDWR, (unsigned long)&e2prom_data);  
   */  
    Single_WriteL3G4200D(0x10, 0x30);
    Single_WriteL3G4200D(0x20, 0x60);
    Single_WriteL3G4200D(0x21, 0x61);
    Single_WriteL3G4200D(0x22, 0x62);
    Single_WriteL3G4200D(0x23, 0x63);

    //4.构造向eeprom读取数据的消息
    e2prom_data.nmsgs = 2;   //消息个数
    e2prom_data.msgs[0].len = 1;  //字节长度,不包括从机地址,因为初始化已经写于从机地址
    e2prom_data.msgs[0].flags = 0;  //写方向
    e2prom_data.msgs[0].addr = 0x50; //从机地址
    e2prom_data.msgs[0].buf[0] = 0x10;  //写入字节地址
      
    e2prom_data.msgs[1].len = 1;  //字节长度,不包括从机地址,因为初始化已经写于从机地址
    e2prom_data.msgs[1].flags = 1;  //读方向
    e2prom_data.msgs[1].addr = 0x50; //从机地址
    e2prom_data.msgs[1].buf = (unsigned char *)malloc(2);  //为buf申请空间
     if(NULL ==  e2prom_data.msgs[1].buf )
        printf("get the recievebuf fail!\n");
    e2prom_data.msgs[1].buf[0] = 0;  //接收到的数据存放
  
    //5.从eeprom中读出数据,unsigned long注意必须是unsigned long的 地址
    ioctl(fd, I2C_RDWR, (unsigned long)&e2prom_data); 
    
    printf("the data ls 0x%x\n",  e2prom_data.msgs[1].buf[0]);
    
    //////////////////////////////////////////////////////////
    
    e2prom_data.nmsgs = 2;   //消息个数
    e2prom_data.msgs[0].len = 1;  //字节长度,不包括从机地址,因为初始化已经写于从机地址
    e2prom_data.msgs[0].flags = 0;  //写方向
    e2prom_data.msgs[0].addr = 0x50; //从机地址
    e2prom_data.msgs[0].buf[0] = 0x20;  //写入字节地址
      
    e2prom_data.msgs[1].len = 1;  //字节长度,不包括从机地址,因为初始化已经写于从机地址
    e2prom_data.msgs[1].flags = 1;  //读方向
    e2prom_data.msgs[1].addr = 0x50; //从机地址
    e2prom_data.msgs[1].buf = (unsigned char *)malloc(2);  //为buf申请空间
     if(NULL ==  e2prom_data.msgs[1].buf )
        printf("get the recievebuf fail!\n");
    e2prom_data.msgs[1].buf[0] = 0;  //接收到的数据存放
  
    //5.从eeprom中读出数据,unsigned long注意必须是unsigned long的 地址
    ioctl(fd, I2C_RDWR, (unsigned long)&e2prom_data); 
    
    printf("the data ls 0x%x\n",  e2prom_data.msgs[1].buf[0]);
    
    e2prom_data.nmsgs = 2;   //消息个数
    e2prom_data.msgs[0].len = 1;  //字节长度,不包括从机地址,因为初始化已经写于从机地址
    e2prom_data.msgs[0].flags = 0;  //写方向
    e2prom_data.msgs[0].addr = 0x50; //从机地址
    e2prom_data.msgs[0].buf[0] = 0x21;  //写入字节地址
      
    e2prom_data.msgs[1].len = 1;  //字节长度,不包括从机地址,因为初始化已经写于从机地址
    e2prom_data.msgs[1].flags = 1;  //读方向
    e2prom_data.msgs[1].addr = 0x50; //从机地址
    e2prom_data.msgs[1].buf = (unsigned char *)malloc(2);  //为buf申请空间
     if(NULL ==  e2prom_data.msgs[1].buf )
        printf("get the recievebuf fail!\n");
    e2prom_data.msgs[1].buf[0] = 0;  //接收到的数据存放
  
    //5.从eeprom中读出数据,unsigned long注意必须是unsigned long的 地址
    ioctl(fd, I2C_RDWR, (unsigned long)&e2prom_data); 
    
    printf("the data ls 0x%x\n",  e2prom_data.msgs[1].buf[0]);
    
    e2prom_data.nmsgs = 2;   //消息个数
    e2prom_data.msgs[0].len = 1;  //字节长度,不包括从机地址,因为初始化已经写于从机地址
    e2prom_data.msgs[0].flags = 0;  //写方向
    e2prom_data.msgs[0].addr = 0x50; //从机地址
    e2prom_data.msgs[0].buf[0] = 0x22;  //写入字节地址
      
    e2prom_data.msgs[1].len = 1;  //字节长度,不包括从机地址,因为初始化已经写于从机地址
    e2prom_data.msgs[1].flags = 1;  //读方向
    e2prom_data.msgs[1].addr = 0x50; //从机地址
    e2prom_data.msgs[1].buf = (unsigned char *)malloc(2);  //为buf申请空间
     if(NULL ==  e2prom_data.msgs[1].buf )
        printf("get the recievebuf fail!\n");
    e2prom_data.msgs[1].buf[0] = 0;  //接收到的数据存放
  
    //5.从eeprom中读出数据,unsigned long注意必须是unsigned long的 地址
    ioctl(fd, I2C_RDWR, (unsigned long)&e2prom_data); 
    
    printf("the data ls 0x%x\n",  e2prom_data.msgs[1].buf[0]);
    
    
   e2prom_data.nmsgs = 2;   //消息个数
    e2prom_data.msgs[0].len = 1;  //字节长度,不包括从机地址,因为初始化已经写于从机地址
    e2prom_data.msgs[0].flags = 0;  //写方向
    e2prom_data.msgs[0].addr = 0x50; //从机地址
    e2prom_data.msgs[0].buf[0] = 0x23;  //写入字节地址
      
    e2prom_data.msgs[1].len = 1;  //字节长度,不包括从机地址,因为初始化已经写于从机地址
    e2prom_data.msgs[1].flags = 1;  //读方向
    e2prom_data.msgs[1].addr = 0x50; //从机地址
    e2prom_data.msgs[1].buf = (unsigned char *)malloc(2);  //为buf申请空间
     if(NULL ==  e2prom_data.msgs[1].buf )
        printf("get the recievebuf fail!\n");
    e2prom_data.msgs[1].buf[0] = 0;  //接收到的数据存放
  
    //5.从eeprom中读出数据,unsigned long注意必须是unsigned long的 地址
    ioctl(fd, I2C_RDWR, (unsigned long)&e2prom_data); 
    
    printf("the data ls 0x%x\n",  e2prom_data.msgs[1].buf[0]);
  
    //6.关闭设备
    close(fd);
    
    return 0;	
}


/*往l3g写入一个字节*/
void Single_WriteL3G4200D(unsigned char REG_Address,unsigned char REG_data)
{
    /*1. 构建消息*/
    e2prom_data.nmsgs = 1;   //写操作
    e2prom_data.msgs[0].len = 2;  //写入数据的字节长度,
    e2prom_data.msgs[0].flags = 0;  //写方向
    e2prom_data.msgs[0].addr = SlaveAddress; //设备地址 
    e2prom_data.msgs[0].buf = (unsigned char *)malloc(2);  //为buf申请空间
    if(NULL == e2prom_data.msgs[0].buf)
        printf("get the senfbuf fail!\n"); 
    e2prom_data.msgs[0].buf[0] = REG_Address;  //写入字节地址
    e2prom_data.msgs[0].buf[1] = REG_data;  //写入的数据
    
    /*2.l3g4200发送消息*/
    ioctl(fd, I2C_RDWR, (unsigned long)&e2prom_data);    	
}

/*往l3g读出一个字节*/
unsigned char Single_ReadL3G4200D(unsigned char REG_Address)
{                         
    /*1.写入从机地址和寄存器地址*/   
    e2prom_data.nmsgs = 2;   //消息个数
    e2prom_data.msgs[0].len = 1;  //字节长度,不包括从机地址,因为初始化已经写于从机地址
    e2prom_data.msgs[0].flags = 0;  //写方向
    e2prom_data.msgs[0].addr = SlaveAddress; //从机地址
    e2prom_data.msgs[0].buf = (unsigned char *)malloc(1);  //为buf申请空间
    if(NULL == e2prom_data.msgs[0].buf)
         printf("get the senfbuf fail!\n");
    e2prom_data.msgs[0].buf[0] = REG_Address;  //写入字节地址
      
    e2prom_data.msgs[1].len = 1;  //字节长度,不包括从机地址,因为初始化已经写于从机地址
    e2prom_data.msgs[1].flags = 1;  //读方向
    e2prom_data.msgs[1].addr = SlaveAddress; //从机地址
    e2prom_data.msgs[1].buf = (unsigned char *)malloc(2);  //为buf申请空间
    if(NULL ==  e2prom_data.msgs[1].buf )
        printf("get the recievebuf fail!\n");
    e2prom_data.msgs[1].buf[0] = 1;  //接收到的数据存放
  
    /*2.从gy50_data中读出数据*/
    ioctl(fd, I2C_RDWR, (unsigned long)&e2prom_data); 
    
    printf("the data ls %d\n",  e2prom_data.msgs[1].buf[0]);        
  
    return e2prom_data.msgs[1].buf[0]; 
}