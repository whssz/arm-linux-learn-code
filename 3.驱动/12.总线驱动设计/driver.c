#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>

extern struct bus_type my_bus_type;

int my_probe(struct device *dev)
{
    printk("the dev is found!\n");
    return 0;  	
}

struct device_driver my_driver=
{
    .name = "my_dev",
    .bus = &my_bus_type,
    .probe = my_probe,    	
};

static int my_driver_init()
{  
    int ret = driver_register(&my_driver);
    
    return ret;	
}

static void my_driver_exit()
{
    driver_unregister(&my_driver);
}

MODULE_LICENSE("GPL");
module_init(my_driver_init);
module_exit(my_driver_exit); 