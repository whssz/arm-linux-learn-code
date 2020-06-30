/************************************
*��������:2016-11-5
*�ļ���: DHT11���������
*�޸�: delay����.������ȷ.���¶�����ƫ��.
*/

#define  GPGCON  (*(volatile unsigned long *)0x56000060)
#define  GPGDAT  (*(volatile unsigned long *)0x56000064)
#define  GPGUP   (*(volatile unsigned long *)0x56000068)

/*IO��������*/
#define DHT11_IO_IN()     {GPGCON &= ~(0x11<<2);} 
#define DHT11_IO_OUT()    {GPGCON &= ~(0x11<<2); GPGCON |= 0x1<<2;}	
/*IO�����ƽ*/
#define DHT11_DATA_UP()   {GPGDAT |= 0x1<<1;} 
#define DHT11_DATA_DOWN() {GPGDAT &= ~(0x1<<1);}
 
/*��ȡIO״̬*/
#define	DHT11_DATA_GET    GPGDAT&(0x1<<1)							   


/*��ʱ����, ���㵥λΪ1.03ms*/
void Delay_ms(int tt)
{
    int i;
    for( ; tt>0; tt--)
    {
        for(i=0; i<620; i++){}
    }
}

/*��ʱ����, ���㵥λΪ14us*/
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

//��λDHT11
void DHT11_Rst(void)	   
{                 
    DHT11_IO_OUT(); 	   //G1����Ϊ���
    DHT11_DATA_DOWN();     //����G1
    Delay_ms(20);    	  //��������18ms
    DHT11_DATA_UP();      //����,�ȴ��ӻ���Ӧ
    Delay_us(2);         //��������20~40us	 	
}

//�ȴ�DHT11�Ļ�Ӧ
//����1:δ��⵽DHT11�Ĵ���
//����0:����
unsigned char DHT11_Check(void) 	   
{   
    unsigned char retry=0;
    DHT11_IO_IN();  //���� IO Ϊ����	 
    while (DHT11_DATA_GET&&retry<7)//DHT11������40~80us
    {
	retry++;
	Delay_us(1);
    };	 
    if(retry>=7)return 1;
    else retry=0;
    
    while (!DHT11_DATA_GET&&retry<7)//DHT11���ͺ���ٴ�����40~80us
    {
	retry++;
	Delay_us(1);
    };
    if(retry>=7)return 1;	    
    return 0;
}


//��DHT11��ȡһ��λ
//����ֵ��1/0
unsigned char DHT11_Read_Bit(void) 			 
{
 	unsigned char retry=0;
 	
	while(DHT11_DATA_GET)//�ȴ���Ϊ�͵�ƽ
	{
		retry++;
		//Delay_us(1);
	}
	//if(retry>=7)return 1;
	retry=0;
	while(!DHT11_DATA_GET)//�ȴ���ߵ�ƽ
	{
		retry++;
		//Delay_us(1);
	}
	//if(retry>=7)return 1;
	retry =0;
	Delay_us(4);//�ȴ�40us
	
	if(DHT11_DATA_GET)
	    return 1;
	else return 0;		   
}

//��DHT11��ȡһ���ֽ�
//����ֵ������������
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

//��DHT11��ȡһ������
//temp:�¶�ֵ(��Χ:0~50��)
//humi:ʪ��ֵ(��Χ:20%~90%)
//����ֵ��0,����;1,��ȡʧ��
unsigned char DHT11_Read_Data(unsigned char *temp,unsigned char *humi)    
{        
 	unsigned char  buf[5];
	unsigned char  i;
	DHT11_Rst();
	if(DHT11_Check()==0)
	{
		for(i=0;i<5;i++)//��ȡ40λ����
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

/*��ʼ������*/
unsigned char dht_port_init(void)
{
    int ret=0;
    /*1.��������Ϊ���*/
    GPGCON &= ~(0x11<<2);
    GPGCON |= 0x1<<2;
    
    /*2.��ʼ״̬Ϊ�ߵ�ƽ*/
    GPGDAT |= 0x1<<1;
    
    /*3.�������迪��*/
    GPGUP &= ~(0x1<<1);
    
    /*4.��λһ��,�ȴ��ӻ���Ӧ*/
    DHT11_Rst();
    
    /*5.���ӻ��Ƿ�Ӧ��*/
    ret = DHT11_Check();
    
    return ret;
}

