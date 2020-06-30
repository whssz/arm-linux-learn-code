#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>

extern struct bus_type my_bus_type;

 struct device my_device=
 {
     .init_name = "my_dev",
     .bus = &my_bus_type,	
 };
 
static int my_device_init()
{   
    int ret = device_register(&my_device);
    return ret;	
}


static void my_device_exit()
{
    device_unregister(&my_device);
}

MODULE_LICENSE("GPL");
module_init(my_device_init);
module_exit(my_device_exit);