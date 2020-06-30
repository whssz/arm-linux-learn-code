#define GPBCON  (volatile unsigned long*)0x56000010
#define GPBDAT  (volatile unsigned long*)0x56000014
#define GPFCON  (volatile unsigned long*)0x56000050

#define GPFDAT  (volatile unsigned char*)0x56000054
#define GPFUP   (volatile unsigned char*)0x56000058



void delayms(int x)
{  
    int i,j;
    for(i=x; i>0; i--)
        for(j=1000; j>0; j--);
}

 /*1.��ʼ������*/
void port_init()
{
    /*1.1 ���������*/
    *(GPBCON) &= ~(3<<0);
    *(GPBCON) |= 0b1;   
    *(GPBDAT) &= (~0x1);
    /*1.2 ����������������Ϊ��ƽ������ʽ*/
   
    *(GPFCON) &= ~(3<<2);
    *(GPFCON) |=  (0x2<<2);
    *(GPFUP)  |=  (0x1<<1); //��ֹ����
    *(GPFDAT) &= ~(0x1<<1);   
}

/*�жϷ�����*/
void humen_interrpt()
{
     unsigned int i,j;
     
     led_on(); 
    
     delayms(300);
     *(GPBDAT) |= 0x1; 
     
     led_off(); 
     delayms(300); 
    	
}

void reset_beep()
{
   *(GPBDAT) &= (~0x1);	
}
