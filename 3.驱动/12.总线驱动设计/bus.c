#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>


int my_bus_match(struct device *dev, struct device_driver *drv)
{  
    int ret = !strncmp(dev->kobj.name, drv->name, strlen(drv->name)); //��ͬ����0, ����,���ش���0��ֵ, С��,С��0��ֵ
    return ret; //û��ƥ�䷵��0
}

struct bus_type my_bus_type=
{
 .name = "my_bus",
 .match = my_bus_match,
};

EXPORT_SYMBOL(my_bus_type); //�������


static int my_bus_init()
{   
    int ret;
    /*ע������*/
    ret = bus_register(&my_bus_type);
   
    return ret;	
}

static void my_bus_exit()
{
    /*ע������*/
    bus_unregister(&my_bus_type);
}

MODULE_LICENSE("GPL");
module_init(my_bus_init);
module_exit(my_bus_exit);