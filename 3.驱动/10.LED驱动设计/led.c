#include <linux/module.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/io.h>
#include "led.h"

#define GPBCON   0x56000010
#define GPBDAT   0x56000014

unsigned int *led_config;
unsigned int *led_data;

struct cdev cdev;
dev_t devno;


int led_open(struct inode *node, struct file *filp)
{
    /*初始化led ,GPB5678*/
    led_config = ioremap(GPBCON, 4); //物理地址转化为虚拟地址
    writel(0x15400, led_config);   //向虚拟地址写入数据
    
    led_data = ioremap(GPBDAT, 4); 
    
    return 0; 
}

long led_ioctrl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    /*定义命令*/	   
    /*swith判断指令类型*/
    switch (cmd)
    {
       case LED_ON:
           writel(0x61f, led_data);  
           return 0;
          
       case LED_OFF:
           writel(0x3ff, led_data); 
           return 0;
          
       default:
           return  -EINVAL;         	
    }
}


static struct file_operations  led_fops=
{
    .open = led_open,
    .unlocked_ioctl = led_ioctrl,
};

static int led_init(void)
{  
    /*分配cdev结构体*/
    /*初始化cdev*/
    cdev_init(&cdev, &led_fops);
    /*注册cdev*/
     alloc_chrdev_region(&devno, 0, 1, "myled");
    cdev_add(&cdev, devno , 1);
    
    return 0; 	
}

static void led_exit(void)
{
    /*注销cdev*/
    cdev_del(&cdev);
    /*释放主设备号*/
    unregister_chrdev_region(devno, 1);
}

MODULE_LICENSE("GPL");

module_init(led_init);
module_exit(led_exit);