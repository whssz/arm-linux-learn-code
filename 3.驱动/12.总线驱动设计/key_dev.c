#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>


#define GPGCON 0x56000060
#define GPGDAT 0x56000064


struct resource key_resource[]=
{
    [0] = {
    	      .start = GPGCON,
    	      .end = GPGCON +8, //数据和控制寄存器
    	      .flags = IORESOURCE_MEM,
    	  },
    	  
    [1] = {
    	      .start = IRQ_EINT8,
    	      .end = IRQ_EINT11,
    	      .flags = IORESOURCE_IRQ,
    	  }  
};

struct platform_device key_device=
{
   .name = "key_bus" ,
   .id = 0,
   .num_resources = 2,
   .resource = 	key_resource,
};


int keydev_init()
{ 
    platform_device_register(&key_device);
    return 0;	
}

void keydev_exit()
{
    platform_device_unregister(&key_device);	
}


MODULE_LICENSE("GPL");
module_init(keydev_init);
module_exit(keydev_exit);