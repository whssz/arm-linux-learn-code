#define GPECON  *(volatile unsigned long*)0x56000040
#define GPEUP   *(volatile unsigned long*)0x56000048
#define SRCPND  *(volatile unsigned long*)0X4a000000
#define INTPND  *(volatile unsigned long*)0X4a000010
#define INTMSK  *(volatile unsigned long*)0X4a000008

#define IICCON  *(volatile unsigned char*)0x54000000
#define IICSTAT *(volatile unsigned char*)0x54000004
#define IICDS   *(volatile unsigned char*)0x5400000c

#define SLAVE_WRITE_ADD 0xa0
#define SLAVE_READ_ADD 0xa1


void delay(unsigned int dl_time)
{
    unsigned j;
    while(dl_time--)
    {
        for(j=100;j>0;j--);	
    }
}
void icc_init()
{  
    //4.引脚初始化(静止上拉,引脚复用)
    GPECON |= (2<<28) | (2<<30);
    GPEUP |= (3<<14);   
         
    //1.1 打开中断
    IICCON |= 1<<5;
    INTPND |= 1<<27; 
    SRCPND |= 1<<27;
    INTMSK &= ~(1<<27);
    
    //1.2 配置时钟
    IICCON &= ~(1<<6);
    IICCON |= (0xf<<0);
    IICCON |= (0x5<<0);
    
    //2.启动输出
    IICSTAT |= (1<<4);
    
    //3.允许应当ack
     IICCON |= (1<<7);	
}


void write_byte(unsigned char sdata, unsigned char daddr)
{
    //1.设置为主机发送模式
    IICSTAT |= (3<<6);
    
    //2.写入从机数据到IICDS
    IICDS = SLAVE_WRITE_ADD;
    IICCON &= ~(1<<4);
    
    //3.写入0xf0到IICSTAT开启传输
    IICSTAT = 0xf0;
    
    //等待应答,产生应答就会产生中断
    while(0 == (IICCON &(1<<4)))
        delay(100); 
        
    //写入字节地址eeprom的内部地址到从机
    IICDS = daddr;
    IICCON &= ~(1<<4);
    while(0 == (IICCON &(1<<4)))
        delay(100); 
        
    //5.发送数据到IICDS
    IICDS = sdata;
    IICCON &= ~(1<<4);
    while(0 == (IICCON &(1<<4)))
        delay(100); 
        
    //7.写入0xd0到IICSTAT停止传输
    IICSTAT = 0xd0; 
    
    //8.清除中断 ,可以可无,加上更加保险
    IICCON &= ~(1<<4);
    
    //9.等待结束
    delay(100); 
}



void read_data(unsigned char *buf, unsigned char saddr, unsigned short length)
{   
     unsigned char nus_data;
     unsigned short j=0;
     
    //设置为主机发送模式
     IICSTAT |= (3<<6);
     
    //发送从机地址  
     IICDS = SLAVE_WRITE_ADD;
     IICCON &= ~(1<<4);
     
   //写入0xF0到IICSTAT
    IICSTAT = 0xF0;
    
    //等待应答
    while(0 == (IICCON &(1<<4)))
        delay(100); 
        
    //发送eeprom的内部读地址
    IICDS = saddr;  
    IICCON &= ~(1<<4); 
    while(0 == (IICCON &(1<<4)))
        delay(100); 
        
     /******************以上是eeprom的特殊操作*****************/
    //1.设置cpu为主机读模式
    IICSTAT |= (2<<6);
    
    //2.写入从机地址到IICDS
    IICDS = SLAVE_READ_ADD;
    IICCON &= ~(1<<4);
    
    //3.写入数据0xb0到IICSTAT
    IICSTAT = 0xb0;
    //4.等待应答
    
    while(0 == (IICCON &(1<<4)))
        delay(100);
        
    //5.丢弃第一个数据 
    nus_data = IICDS;  //这里知识开始读寄存器,但是寄存器里面还没有需要的值
    IICCON &= ~(1<<4);
    while(0 == (IICCON &(1<<4)))
       delay(100);
       
    for(j=0 ; j<length; j++)
    {   
    	if(j == length-1)
    	{
    	   IICCON &= ~(1<<7);  //关闭应答,以结束传输 	
    	}
        //5.读取数据
        buf[j] = IICDS;     
        IICCON &= ~(1<<4);        
        while(0 == (IICCON &(1<<4)))
            delay(100);  
                
    }
    //写入0x90到IICSTAT中
     IICSTAT = 0x90;
     
    // 清除中断
     IICCON &= ~(1<<4);
}
  
 
 
void i2c_test()
{  
    unsigned char arg1[10]={0};
    unsigned char arg2[10]={0};
    int i=0;
    
    icc_init();
     
    for(i=0; i<10;i++)
    {
        arg1[i] = i+1; 
        //if(0 == i%4)
            //printf("\n");  //\r 回车 \n换行
        printf("%d ", arg2[i]);        
    }
    printf("\n\r");
    printf("data is write........\n\r");
    
    //装入数据
     for(i=0; i<10;i++)
     {
         write_byte(arg1[i], i);	
     }
     
    //读出数据
    read_data(arg2, 0, 10);
 
    printf("the read out data is:\n\r");
    
    for(i=0; i<10;i++)
    {
         //if(0 == i%8)
           // printf("\n\r");  //\r 回车 \n换行
         printf("%d ", arg2[i]);   	
    }	
     printf("\n\r");	
}