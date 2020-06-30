#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>

#define   GPGCON  0x56000060


struct resource	key_resoure[]=
{
    [0] = {
    	      .start = GPGCON,
    	      .end = GPGCON + 8,
    	      .flags  = IORESOURCE_MEM, 
    	  },
    	  
    [1] = {
    	      .start = IRQ_EINT8,
    	      .end = IRQ_EINT12,
    	      .flags  = IORESOURCE_IRQ, 
    	  },
    	  
   // [2]= {
    	    // .start = IRQ_EINT11,
    	    // .end = IRQ_EINT11,
    	    // .flags  = IORESOURCE_IRQ, 
    	// },   	  
};


struct platform_device key_device=
{
    .name = "my_key",
    .id = 0, 
    .num_resources = 2,
    .resource = key_resoure,	
};

static int key_init(void)
{   
    platform_device_register(&key_device);
    
    return 0;
}

static void key_exit(void)
{
    platform_device_unregister(&key_device);	
}
module_init(key_init);
module_exit(key_exit);

MODULE_LICENSE("GPL");