#define GPHCON      (*(volatile unsigned long*)0x56000070)

#define ULCON0      (*(volatile unsigned long*)0x50000000)
#define ULCON1      (*(volatile unsigned long*)0x50004000)

#define UCON0       (*(volatile unsigned long*)0x50000004)
#define UCON1       (*(volatile unsigned long*)0x50004004)

#define UBRDIV0     (*(volatile unsigned long*)0x50000028)
#define UTRSTAT0    (*(volatile unsigned long*)0x50000010)
#define UBRDIV1     (*(volatile unsigned long*)0x50004028)
#define UTRSTAT1    (*(volatile unsigned long*)0x50004010)


#define UTXH0       (*(volatile unsigned long*)0x50000020)
#define URXH0       (*(volatile unsigned long*)0x50000024)
#define UTXH1       (*(volatile unsigned long*)0x50004020)
#define URXH1       (*(volatile unsigned long*)0x50004024)


#define PCLK   50000000  // 串口挂载在APB总线，所以用PCLK
#define BAUD   9600 
#define BAUD2  9600
void uart_init() //串口初始化
{
  GPHCON &= ~(0xff << 4);  //首先清0
  GPHCON |= (0xaa << 4);  //设置引脚输入输出 
  //设数据格式
  ULCON0 = 0b11; 
  ULCON1 = 0b11; 
  //设置工作模式
  UCON0 =  0b101;
  UCON1 =  0b101;
  // 设置波特率
  UBRDIV0 = (int)(PCLK /(BAUD2 *16))-1;  //设置波特率，固定公式
  UBRDIV1 = (int)(PCLK /(BAUD*16))-1; 
}



void send_data(unsigned char ch)
{
   while (!(UTRSTAT0 & (1<<1)));
   UTXH0 = ch;  
}

unsigned char receive_data(void)
{ 
	unsigned char ret;

	while (!(UTRSTAT0 & (1<<0)));
	// 取数据
	ret = URXH0;  
	
	if ( (ret == 0x0d) || (ret == 0x0a) )
	{
	    send_data(0x0d);
	    send_data(0x0a);	
	}      	
	else
	    send_data(ret);
	    
        return ret;
}

void send_data1(unsigned char ch)
{
   while (!(UTRSTAT1 & (1<<1)));
   UTXH1 = ch;  
}

unsigned char receive_data1(void)
{ 
	unsigned char ret1;

	while (!(UTRSTAT1 & (1<<0)));
	// 取数据
	ret1 = URXH1;  
	
	/*if ( (ret1 == 0x0d) || (ret1 == 0x0a) )
	{
	    send_data1(0x0d);
	    send_data1(0x0a);	
	}      	
	else
	    send_data1(ret1);
	 */ 
        return ret1;
}
