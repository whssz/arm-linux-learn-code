#define  GPGCON    (volatile unsigned long*)0x56000060
#define  GPECON    (volatile unsigned long*)0x56000040
#define  SPCON0    (volatile unsigned long*)0x59000000
#define  SPSTA0    (volatile unsigned long*)0x59000004
#define  SPPIN0    (volatile unsigned long*)0x59000008
#define  SPPRE0    (volatile unsigned long*)0x5900000c
#define  SPTDAT0   (volatile unsigned long*)0x59000010
#define  SPRDAT0   (volatile unsigned long*)0x59000014

#define READY_WRRD  *(SPSTA0)&0x1
void spi_port_init()
{  
    /*1. 配置引脚, 默认上拉*/
    *(GPECON) &= ~(0x3f<<22);
    *(GPECON) |= 0x2a<<22;
    *(GPGCON) |=0x3<<4;
    
    /*2. 配置寄存器*/
    *(SPCON0) = 0x1a;
    *(SPPRE0) = 0x18; 
}


void writeb(char c)
{
    while(!READY_WRRD);
    *(SPTDAT0) = c;   	
}

char readb()
{  
    char c=0;
    //while(!READY_WRRD);
    /*发送0xff, 启动传输*/	
  // *(SPTDAT0) = 0xFF;
   while(!READY_WRRD);
   c = *(SPRDAT0); 
   return c;
}


