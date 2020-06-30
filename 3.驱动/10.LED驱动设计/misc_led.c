#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/io.h>
#include "myled.h"

#define GPBCON   0x56000010
#define GPBDAT   0x56000014

unsigned int *led_con;
unsigned int *led_data;



/*打开设备*/
static int led_open(struct inode *node, struct file *filp)
{
    /*获取寄存器映射地址*/
    led_con = ioremap(GPBCON, 4);
    if(!led_con)
    {
        printk(KERN_WARNING"led_con ioremap fail\n");
        goto err_map;	
    }
    
    led_data = ioremap(GPBDAT, 4);
    if(!led_data)
    {
        printk(KERN_WARNING"led_data ioremap fail\n");
        goto err_map;	
    }
    /*写入寄存器值*/ 
    writel(0x15400, led_con);
    
    return 0;
         
    err_map :
    	    return -EFAULT;    	
}
/*设备控制*/
long led_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    switch(cmd)
    {
        case LED_ON:
            writel(0x61f,led_data);
            return 0;
            
        case LED_OFF:
            writel(0x3ff,led_data);	
            return 0;
         
        default:
            return -EINVAL;
    }	
}


/*初始化file_operation*/
struct file_operations led_fops=
{
    .open = led_open,
    .unlocked_ioctl = led_ioctl,	
};

/*1.静态分配miscdevice结构*/
struct miscdevice led_miscdev= 
{
    .minor = 200,
    .name = "my_led",  //设备节点名 "/dev/"
    .fops = &led_fops,	
};

static int __init led_init(void)
{
     int ret;
 
     //2.注册misc设备驱动
     ret =  misc_register(&led_miscdev);
     if(ret < 0)
     {   
     	 printk(KERN_WARNING"add miscdev fial!\n");
         goto err_add;
     }
          
    return 0;
    	
    err_add : misc_deregister(&led_miscdev);   
    return ret;
}


static void __exit led_exit(void)
{   
   misc_deregister(&led_miscdev);    
}

module_init(led_init);
module_exit(led_exit);

MODULE_LICENSE("GPL");