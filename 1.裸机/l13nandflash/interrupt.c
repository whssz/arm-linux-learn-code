#define INTMSK            (volatile unsigned long *)0x4a000008
#define EINTMASK          (volatile unsigned long *)0x560000A4
#define INTOFFSET         (volatile unsigned long *)0x4A000014
#define EINTPEND          (volatile unsigned long *)0x560000A8
#define INTPND            (volatile unsigned long *)0x4a000010
#define SRCPND            (volatile unsigned long *)0x4A000000


void irq_init()
{  //配置EINTMASK寄存器
   *(EINTMASK) &= (~(1<<8)) & (~(1<<11)) & (~(1<<13)) & (~(1<<14));
  //配置INT寄存器
   *(INTMSK)   &= ~(1<<5);
  //开启中断，在cpsr的c域，第七位 
   __asm__(
    "mrs r0,cpsr\n"
    "bic r0, r0, #0x80\n"   //把第七位置0
    "msr cpsr_c, r0\n"      
    : 
    : 
   );   
}



void handle_int()
{  //判断中断源
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
    
    //清除中断标志，在相应位写1   
     *(EINTPEND) = value;
     
     *(SRCPND) = 1 << 5;
     *(INTMSK) = 1 << 5;
}
