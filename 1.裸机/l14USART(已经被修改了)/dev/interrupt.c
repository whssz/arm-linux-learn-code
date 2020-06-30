#define INTMSK            (volatile unsigned long *)0x4a000008
#define EINTMASK          (volatile unsigned long *)0x560000A4
#define INTOFFSET         (volatile unsigned long *)0x4A000014
#define EINTPEND          (volatile unsigned long *)0x560000A8
#define INTPND            (volatile unsigned long *)0x4a000010
#define SRCPND            (volatile unsigned long *)0x4A000000


void irq_init()
{  //����EINTMASK�Ĵ���
   *(EINTMASK) &= (~(1<<8)) & (~(1<<11)) & (~(1<<13)) & (~(1<<14));
  //����INT�Ĵ���
   *(INTMSK)   &= ~(1<<5);
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
   unsigned long value = *(EINTPEND) & ((1<<8)|(1<<11)|(1<<13)|(1<<14));	
   
   switch(value)
   {
       case (1<<8): //k1
       led_on();
       break;
       
       case (1<<11): //k2
       led_off();
       break;
       
       case (1<<13)://k3
       led_on();
       break;
       
       case(1<<14)://k4
       led_off();
       break;
       
       default :
       break;   	 	
    }  
    
    //����жϱ�־������Ӧλд1   
     *(EINTPEND) = value;
     
     *(SRCPND) = 1 << 5;
     *(INTMSK) = 1 << 5;
}
