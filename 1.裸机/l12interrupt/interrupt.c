#define INTMSK            (volatile unsigned long *)0x4a000008
#define EINTMASK          (volatile unsigned long *)0x560000A4
#define INTOFFSET         (volatile unsigned long *)0x4A000014
#define EINTPEND          (volatile unsigned long *)0x560000A8
#define INTPND            (volatile unsigned long *)0x4a000010
#define SRCPND            (volatile unsigned long *)0x4A000000
#define EXTINT0           (volatile unsigned long*)0x56000088


void irq_init()
{  //配置EINTMASK寄存器
   *(EINTMASK) &= ~(1<<8);
  //配置INT寄存器
   *(INTMSK)   = ~(0x22);
   
   /*1.3 设置中断为高电平触发*/
   *(EXTINT0) &= ~(0x7<<4);
   *(EXTINT0) |= 0x1<<4; 
    	
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
    
    //清除中断标志，在相应位写1   
    if(value == 5)
	*(EINTPEND) = (1 << 5);
     
     if(value == 1)
     {
         reset_beep();	
     }
     
     *(SRCPND) = 1 << value;  /*必须先清除SRCPND*/
     *(INTPND) = 1 << value;
   
}
