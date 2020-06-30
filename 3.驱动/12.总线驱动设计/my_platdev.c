#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>

#define GPBCON   0x56000010

struct resource my_resource[]=
{
    [0] = {
    	      .start = GPBCON,
    	      .end = GPBCON + 8,
    	      .flags = IORESOURCE_MEM,
    	  },	
};

struct platform_device my_dev=
{
    .name = "my_dev",
    .id = 0,
    .num_resources = 1,
    .resource = my_resource,	
};

static int __init mydev_init(void)
{
    platform_device_register(&my_dev);
    return 0;	
}

static void __exit mydev_exit(void)
{
    platform_device_unregister(&my_dev);	
}

module_init(mydev_init);
module_exit(mydev_exit);

MODULE_LICENSE("GPL");