/************************************
*创建日期:2016-11-5
*文件名: DHT11的裸机程序
*修改: delay函数.不够精确.导致读数有偏差.
*/

#define  GPGCON  (*(volatile unsigned long *)0x56000060)
#define  GPGDAT  (*(volatile unsigned long *)0x56000064)
#define  GPGUP   (*(volatile unsigned long *)0x56000068)

/*IO方向设置*/
#define DHT11_IO_IN()     {GPGCON &= ~(0x11<<2);} 
#define DHT11_IO_OUT()    {GPGCON &= ~(0x11<<2); GPGCON |= 0x1<<2;}	
/*IO输出电平*/
#define DHT11_DATA_UP()   {GPGDAT |= 0x1<<1;} 
#define DHT11_DATA_DOWN() {GPGDAT &= ~(0x1<<1);}
 
/*获取IO状态*/
#define	DHT11_DATA_GET    GPGDAT&(0x1<<1)							   


/*延时函数, 换算单位为1.03ms*/
void Delay_ms(int tt)
{
    int i;
    for( ; tt>0; tt--)
    {
        for(i=0; i<620; i++){}
    }
}

/*延时函数, 换算单位为14us*/
void Delay_us(int tt)
{
    int i;
    for( ; tt>0; tt--)
    {
        for(i=0; i<5; i++){}
    }
}

void delay_test()
{
    DHT11_DATA_UP();
    Delay_us(1);  /*65*/
    DHT11_DATA_DOWN();
    Delay_us(1);
}

//复位DHT11
void DHT11_Rst(void)	   
{                 
    DHT11_IO_OUT(); 	   //G1设置为输出
    DHT11_DATA_DOWN();     //拉低G1
    Delay_ms(20);    	  //拉低至少18ms
    DHT11_DATA_UP();      //拉高,等待从机回应
    Delay_us(2);         //主机拉高20~40us	 	
}

//等待DHT11的回应
//返回1:未检测到DHT11的存在
//返回0:存在
unsigned char DHT11_Check(void) 	   
{   
    unsigned char retry=0;
    DHT11_IO_IN();  //设置 IO 为输入	 
    while (DHT11_DATA_GET&&retry<7)//DHT11会拉低40~80us
    {
	retry++;
	Delay_us(1);
    };	 
    if(retry>=7)return 1;
    else retry=0;
    
    while (!DHT11_DATA_GET&&retry<7)//DHT11拉低后会再次拉高40~80us
    {
	retry++;
	Delay_us(1);
    };
    if(retry>=7)return 1;	    
    return 0;
}


//从DHT11读取一个位
//返回值：1/0
unsigned char DHT11_Read_Bit(void) 			 
{
 	unsigned char retry=0;
 	
	while(DHT11_DATA_GET)//等待变为低电平
	{
		retry++;
		//Delay_us(1);
	}
	//if(retry>=7)return 1;
	retry=0;
	while(!DHT11_DATA_GET)//等待变高电平
	{
		retry++;
		//Delay_us(1);
	}
	//if(retry>=7)return 1;
	retry =0;
	Delay_us(4);//等待40us
	
	if(DHT11_DATA_GET)
	    return 1;
	else return 0;		   
}

//从DHT11读取一个字节
//返回值：读到的数据
unsigned char DHT11_Read_Byte(void)    
{        
    unsigned char i,dat;
    dat=0;
    for (i=0;i<8;i++) 
    {
   	dat<<=1; 
	dat|=DHT11_Read_Bit();
    }						    
    return dat;
}

//从DHT11读取一次数据
//temp:温度值(范围:0~50°)
//humi:湿度值(范围:20%~90%)
//返回值：0,正常;1,读取失败
unsigned char DHT11_Read_Data(unsigned char *temp,unsigned char *humi)    
{        
 	unsigned char  buf[5];
	unsigned char  i;
	DHT11_Rst();
	if(DHT11_Check()==0)
	{
		for(i=0;i<5;i++)//读取40位数据
		{
			buf[i]=DHT11_Read_Byte();
		}
		//if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
		//{
			*humi=buf[0];
			*temp=buf[2];
		//}
	}else return 1;
	return 0;	    
}

/*初始化函数*/
unsigned char dht_port_init(void)
{
    int ret=0;
    /*1.配置引脚为输出*/
    GPGCON &= ~(0x11<<2);
    GPGCON |= 0x1<<2;
    
    /*2.初始状态为高电平*/
    GPGDAT |= 0x1<<1;
    
    /*3.上拉电阻开启*/
    GPGUP &= ~(0x1<<1);
    
    /*4.复位一次,等待从机响应*/
    DHT11_Rst();
    
    /*5.检测从机是否应答*/
    ret = DHT11_Check();
    
    return ret;
}

