#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>


int my_bus_match(struct device *dev, struct device_driver *drv)
{  
    int ret = !strncmp(dev->kobj.name, drv->name, strlen(drv->name)); //相同返回0, 大于,返回大于0的值, 小于,小于0的值
    return ret; //没有匹配返回0
}

struct bus_type my_bus_type=
{
 .name = "my_bus",
 .match = my_bus_match,
};

EXPORT_SYMBOL(my_bus_type); //输出变量


static int my_bus_init()
{   
    int ret;
    /*注册总线*/
    ret = bus_register(&my_bus_type);
   
    return ret;	
}

static void my_bus_exit()
{
    /*注销总线*/
    bus_unregister(&my_bus_type);
}

MODULE_LICENSE("GPL");
module_init(my_bus_init);
module_exit(my_bus_exit);