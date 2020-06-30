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
    //4.���ų�ʼ��(��ֹ����,���Ÿ���)
    GPECON |= (2<<28) | (2<<30);
    GPEUP |= (3<<14);   
         
    //1.1 ���ж�
    IICCON |= 1<<5;
    INTPND |= 1<<27; 
    SRCPND |= 1<<27;
    INTMSK &= ~(1<<27);
    
    //1.2 ����ʱ��
    IICCON &= ~(1<<6);
    IICCON |= (0xf<<0);
    IICCON |= (0x5<<0);
    
    //2.�������
    IICSTAT |= (1<<4);
    
    //3.����Ӧ��ack
     IICCON |= (1<<7);	
}


void write_byte(unsigned char sdata, unsigned char daddr)
{
    //1.����Ϊ��������ģʽ
    IICSTAT |= (3<<6);
    
    //2.д��ӻ����ݵ�IICDS
    IICDS = SLAVE_WRITE_ADD;
    IICCON &= ~(1<<4);
    
    //3.д��0xf0��IICSTAT��������
    IICSTAT = 0xf0;
    
    //�ȴ�Ӧ��,����Ӧ��ͻ�����ж�
    while(0 == (IICCON &(1<<4)))
        delay(100); 
        
    //д���ֽڵ�ַeeprom���ڲ���ַ���ӻ�
    IICDS = daddr;
    IICCON &= ~(1<<4);
    while(0 == (IICCON &(1<<4)))
        delay(100); 
        
    //5.�������ݵ�IICDS
    IICDS = sdata;
    IICCON &= ~(1<<4);
    while(0 == (IICCON &(1<<4)))
        delay(100); 
        
    //7.д��0xd0��IICSTATֹͣ����
    IICSTAT = 0xd0; 
    
    //8.����ж� ,���Կ���,���ϸ��ӱ���
    IICCON &= ~(1<<4);
    
    //9.�ȴ�����
    delay(100); 
}



void read_data(unsigned char *buf, unsigned char saddr, unsigned short length)
{   
     unsigned char nus_data;
     unsigned short j=0;
     
    //����Ϊ��������ģʽ
     IICSTAT |= (3<<6);
     
    //���ʹӻ���ַ  
     IICDS = SLAVE_WRITE_ADD;
     IICCON &= ~(1<<4);
     
   //д��0xF0��IICSTAT
    IICSTAT = 0xF0;
    
    //�ȴ�Ӧ��
    while(0 == (IICCON &(1<<4)))
        delay(100); 
        
    //����eeprom���ڲ�����ַ
    IICDS = saddr;  
    IICCON &= ~(1<<4); 
    while(0 == (IICCON &(1<<4)))
        delay(100); 
        
     /******************������eeprom���������*****************/
    //1.����cpuΪ������ģʽ
    IICSTAT |= (2<<6);
    
    //2.д��ӻ���ַ��IICDS
    IICDS = SLAVE_READ_ADD;
    IICCON &= ~(1<<4);
    
    //3.д������0xb0��IICSTAT
    IICSTAT = 0xb0;
    //4.�ȴ�Ӧ��
    
    while(0 == (IICCON &(1<<4)))
        delay(100);
        
    //5.������һ������ 
    nus_data = IICDS;  //����֪ʶ��ʼ���Ĵ���,���ǼĴ������滹û����Ҫ��ֵ
    IICCON &= ~(1<<4);
    while(0 == (IICCON &(1<<4)))
       delay(100);
       
    for(j=0 ; j<length; j++)
    {   
    	if(j == length-1)
    	{
    	   IICCON &= ~(1<<7);  //�ر�Ӧ��,�Խ������� 	
    	}
        //5.��ȡ����
        buf[j] = IICDS;     
        IICCON &= ~(1<<4);        
        while(0 == (IICCON &(1<<4)))
            delay(100);  
                
    }
    //д��0x90��IICSTAT��
     IICSTAT = 0x90;
     
    // ����ж�
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
            //printf("\n");  //\r �س� \n����
        printf("%d ", arg2[i]);        
    }
    printf("\n\r");
    printf("data is write........\n\r");
    
    //װ������
     for(i=0; i<10;i++)
     {
         write_byte(arg1[i], i);	
     }
     
    //��������
    read_data(arg2, 0, 10);
 
    printf("the read out data is:\n\r");
    
    for(i=0; i<10;i++)
    {
         //if(0 == i%8)
           // printf("\n\r");  //\r �س� \n����
         printf("%d ", arg2[i]);   	
    }	
     printf("\n\r");	
}