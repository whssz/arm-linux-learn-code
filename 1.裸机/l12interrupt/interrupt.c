#define INTMSK            (volatile unsigned long *)0x4a000008
#define EINTMASK          (volatile unsigned long *)0x560000A4
#define INTOFFSET         (volatile unsigned long *)0x4A000014
#define EINTPEND          (volatile unsigned long *)0x560000A8
#define INTPND            (volatile unsigned long *)0x4a000010
#define SRCPND            (volatile unsigned long *)0x4A000000
#define EXTINT0           (volatile unsigned long*)0x56000088


void irq_init()
{  //����EINTMASK�Ĵ���
   *(EINTMASK) &= ~(1<<8);
  //����INT�Ĵ���
   *(INTMSK)   = ~(0x22);
   
   /*1.3 �����ж�Ϊ�ߵ�ƽ����*/
   *(EXTINT0) &= ~(0x7<<4);
   *(EXTINT0) |= 0x1<<4; 
    	
  //�����жϣ���cpsr��c�򣬵���λ 
   __asm__(
    "mrs r0,cpsr\n"
    "bic r0, r0, #0x80\n"   //�ѵ���λ��0
    "msr cpsr_c, r0\n"      
    : 
    : 
   );   
}



void handle_int()
{  //�ж��ж�Դ
   unsigned long value = *(INTOFFSET);	
   
   switch(value)
   {
       case (0): //k1
       led_on();
       break;
       
       case (1): //k2
       humen_interrpt();
       break;
       
       case (5)://k3       	
       led_on();
       break;
       
           
       default :
       break;   	 	
    }  
    
    //����жϱ�־������Ӧλд1   
    if(value == 5)
	*(EINTPEND) = (1 << 5);
     
     if(value == 1)
     {
         reset_beep();	
     }
     
     *(SRCPND) = 1 << value;  /*���������SRCPND*/
     *(INTPND) = 1 << value;
   
}
