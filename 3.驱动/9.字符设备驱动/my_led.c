#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/io.h>
#include "myled.h"

#define GPBCON   0x56000010
#define GPBDAT   0x56000014

unsigned int *led_con;
unsigned int *led_data;


struct cdev led_dev; //1.1静态分配cdev结构
dev_t ledno;

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


/*关闭设备*/
static int led_close(struct inode *node, struct file *filp)
{
    return 0;   	
}

static struct file_operations led_fops= 
{
   
    .open = led_open,   
    .release = led_close,
    .unlocked_ioctl = led_ioctl,
};

static int __init led_init(void)
{
    int ret;
    //1.2 初始化cdev
    cdev_init(&led_dev, &led_fops);
     
    //1.3 注册cdev
    
    //1.3.1动态分配主设备号
     ret = alloc_chrdev_region(&ledno, 0, 1, "myled"); 
     if(ret < 0)
     {
         printk(KERN_WARNING"alloc_chrdev_region() fail\n");
         goto err_dev;	
     } 
     //注册
     ret = cdev_add(&led_dev, ledno, 1);
     if(ret < 0)
     {   
     	 printk(KERN_WARNING"add dev fial!\n");
         goto err_add;
     }
          
    return 0;
    	
    err_add :  cdev_del(&led_dev); 
    err_dev :  unregister_chrdev_region(ledno, 1);
    return ret;
}

static void __exit led_exit(void)
{
    cdev_del(&led_dev); 
    unregister_chrdev_region(ledno, 1); 
}

module_init(led_init);
module_exit(led_exit);

MODULE_LICENSE("GPL");