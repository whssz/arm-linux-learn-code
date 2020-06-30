#include <linux/module.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/platform_device.h>
#include <linux/kthread.h>
#include "my_platform.h"

struct resource *mem_resource;
unsigned int *register_base;

int misc_open(struct inode *node, struct file *filp)
{  
    writel(0x15401, register_base); 
    return 0;	
}

ssize_t misc_write(struct file *filp, const char __user *buf, size_t size, loff_t *f_pos)
{
    return 0;	
}

int misc_release(struct inode *node, struct file *filp)
{
    return 0;	
}

long misc_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{  
    unsigned char dev_type = 0;
    dev_type =  _IOC_TYPE(cmd);
    
    if(dev_type == 'l')
    {   
       switch(cmd)
       {
            case LED_ON:
               writel(0x61e, register_base+1);
               return 0;
            
            case LED_OFF:
                writel(0x7fe, register_base+1);
               return 0;
               
            default:
                break;
       }
     }
       
    if(dev_type == 'b') 
    {
       switch(cmd)
       {
     	   case BEEP_ON:
               writel(0x7ff, register_base+1);
               return 0;
             
           case BEEP_OFF:
               writel(0x7fe, register_base+1);
               return 0;
             
           default:
                break;               
        }
     }  
    return 0;	
}

/*定义混在设备文件描述符*/
const struct file_operations misc_ops=
{
    .open = misc_open,
    .unlocked_ioctl = misc_ioctl,
    .write = misc_write,
    .release = misc_release,
};
/*定义混杂字符设备*/
struct miscdevice misc_driver=
{
    .minor = 0,
    .name = "my_misc_driver",
    .fops = &misc_ops,	
};

/*my_probe*/
int my_probe(struct platform_device *pdev)
{  
    int ret = 0;
    unsigned char size; 
    /*获取资源*/
    mem_resource =  platform_get_resource(pdev, IORESOURCE_MEM, 0);
    if(!mem_resource)
    {
        printk(KERN_WARNING"get the resource fail!\n");
        return -ENODEV;        
    }
    /*转化地址*/
    size = (mem_resource->end) - (mem_resource->start);
    register_base = ioremap(mem_resource->start, size);
    if(!register_base)
    {
         printk(KERN_WARNING"map the register_base fail!\n");
         return -EFAULT;          	
    }
    /*注册混杂设备*/
    ret = misc_register(&misc_driver);
    if(ret)
    {
        goto err_misc;	
    }
      
    return 0;
    
    err_misc: 
        misc_deregister(&misc_driver);
        return ret;
   
 }

/*my_remove*/
static int my_remove(struct platform_device *pdev)
{
    misc_deregister(&misc_driver);
    return 0;	
}

/*分配平台驱动描述符*/
struct platform_driver my_drv=
{
    .probe = my_probe, 
    .remove =  my_remove,
    .driver = {
    	          .owner = THIS_MODULE, 
    		  .name = "my_dev",		  	
              },
};


static int __init mydrv_init(void)
{  
    int ret;
    
    ret = platform_driver_register(&my_drv);
    if(ret)
    {
        printk(KERN_WARNING"add platform driver fail!\n");
        goto err_add;
    }
    
    return 0;
    	
    err_add:
    	platform_driver_unregister(&my_drv); 
        return ret;
}

static void __exit mydrv_exit(void)
{
    platform_driver_unregister(&my_drv);  	
}

module_init(mydrv_init);
module_exit(mydrv_exit);

MODULE_LICENSE("GPL");