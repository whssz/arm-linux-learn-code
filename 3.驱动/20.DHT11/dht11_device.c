#include <linux/module.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/platform_device.h>

#define  GPGCON    0x56000060
#define  GPGDAT    0x56000064


#define GPBCON   0x56000010
#define GPBDAT   0x56000014

struct resource dht_resource[]=
{
    [0] = {
    	      .start = GPGCON,
    	      .end = GPGCON + 12,
    	      .flags = IORESOURCE_MEM,
    	  },
    [1] = {
    	      .start = GPBCON,
    	      .end = GPBCON + 8,
    	      .flags = IORESOURCE_MEM,
    	  },	  	
};

struct platform_device dht_dev=
{
    .name = "dht_dev",
    .id = 0,
    .num_resources = 2,
    .resource = dht_resource,	
};

static int __init dht_init(void)
{
    platform_device_register(&dht_dev);
    return 0;	
}

static void __exit dht_exit(void)
{
   platform_device_unregister(&dht_dev); 	
}

module_init(dht_init);
module_exit(dht_exit);

MODULE_LICENSE("GPL");
