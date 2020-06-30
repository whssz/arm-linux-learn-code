#define GPBCON (volatile unsigned long*)0x56000010
#define GPBDAT (volatile unsigned long*)0x56000014

void led_init()
{ 
   *(GPBCON) |= 0x1<<10; 
   *(GPBDAT) |= 1<<5; 	
}


void led_on()
{
   *(GPBDAT) &= ~(1<<5);
}

void led_off()
{
   *(GPBDAT) |= (1<<5);	
}
