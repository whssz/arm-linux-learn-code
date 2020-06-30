#define SRCPND              (volatile unsigned long *)0x4A000000
#define INTMOD              (volatile unsigned long *)0x4A000004
#define INTMSK              (volatile unsigned long *)0x4A000008
#define PRIORITY            (volatile unsigned long *)0x4A00000c
#define INTPND              (volatile unsigned long *)0x4A000010
#define INTOFFSET           (volatile unsigned long *)0x4A000014
#define SUBSRCPND           (volatile unsigned long *)0x4A000018
#define INTSUBMSK           (volatile unsigned long *)0x4A00001c

#define EINTMASK            (volatile unsigned long *)0x560000a4
#define EINTPEND            (volatile unsigned long *)0x560000a8


void irq_init()
{ 
	 //配置EINTMASK寄存器
   *(EINTMASK) &= ~(1<<8) ;
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
   unsigned long value = *(INTOFFSET);	
   
   switch(value)
   {     
       case 5://k4
       led_on();
       break;
       
       case 31: 
       led_on();     
       //tc_handler();
       break;
       
       
       default :
       break;   	 	
    }  
    
    //清除中断标志，在相应位写1   
     if(value == 5)
     *(EINTPEND) = (1 << 8);
     
     *(SRCPND) = 1 << value;
     *(INTMSK) = 1 << value;
}
