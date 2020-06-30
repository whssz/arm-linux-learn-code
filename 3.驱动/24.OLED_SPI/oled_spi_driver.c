/**************************************************
*文件名: oled_spi_driver.c
*创建日期:2016.11.20
*文件描述: 0.96寸oled(128x64)的spi驱动, 
           linux版本 : 2.6.32,
           oled驱动芯片:SSD1306
           oled描述: 纵轴分为8页,横轴分为128列,每一列在当前页的纵轴上为1个byte, 字节的每一bit代表纵轴上的1个点阵(像素).
           引脚: oled需要dc引脚控制写入 数据/命令, 因此在板级文件的piatform_data中传入了.GPF(3)作为控制引脚.
           学习了对gpio子系统的一部分知识, 能够方便的在内核中使用引脚.
           oled_spi_driver.h : 存放了ioctl命令头文件
*注意: 
     oled清屏过后也是黑色,看上去和没初始化一样,用户程序写入数据过后就能看到现象
     也可以修改驱动中 OLEDClear() 函数的 OLED_Write_Dat(0);把0改到255, 能看到整屏显示白色.
     也可以 电压表测量 驱动芯片 28pin(vcc)  30pin(GND)的电压, 初始化失败: 2.9v左右. 初始化成功: 7.5V左右 
***************************************************/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/gpio.h>
#include <linux/spi/spi.h>
#include <plat/gpio-cfg.h>
#include <asm/uaccess.h>
#include <linux/slab.h>
#include <sound/core.h>
#include <mach/hardware.h>
#include <mach/regs-gpio.h>
#include <linux/types.h>
#include <linux/io.h>
#include <linux/delay.h>
#include <linux/timer.h>
#include "oled_spi_driver.h"


static struct class *oled_class;
static struct spi_device *oled_spi_dev;
static int oled_spi_dc_pin;  
static unsigned char *ker_buf;
static int major;

#define  CDEV_NAME  "oled"

/*dc 引脚设置函数*/
static void OLED_Set_DC(char val)
{
     gpio_set_value(oled_spi_dc_pin, val); 	
}

/*oled 写入命令函数*/
static void OLED_Write_Cmd(unsigned char cmd)
{
    OLED_Set_DC(0); /* command */
    spi_write(oled_spi_dev, &cmd, 1);
    OLED_Set_DC(1);
}

/*oled 写入数据函数*/
static void OLED_Write_Dat(unsigned char dat)
{
    OLED_Set_DC(1); /* data */
    spi_write(oled_spi_dev, &dat, 1);
    OLED_Set_DC(1);
}

/*oled 设置坐标函数*/
static void OLEDSetPos(int page, int col)
{       
    OLED_Write_Cmd(0xB0 + page); /* page address */    
    OLED_Write_Cmd(col & 0xf);   /* Lower Column Start Address */
    OLED_Write_Cmd(0x10 + (col >> 4));   /* Higher Column Start Address */
}

/*oled 设置起始页地址模式*/
static void OLEDSetPageAddrMode(void)
{
   OLED_Write_Cmd(0x20);
   OLED_Write_Cmd(0x02);
}


/*oled 清屏函数*/
static void OLEDClear(void)
{
   int page, i;
   for (page = 0; page < 8; page ++)
   {
       OLEDSetPos(page, 0);
       for(i = 0; i < 128; i++)
       OLED_Write_Dat(0);
   }
}

/*oled 清除单独页函数*/
void OLEDClearPage(int page)
{
   int i;
   OLEDSetPos(page, 0);
   for(i = 0; i < 128; i++)
       OLED_Write_Dat(0);    
}  
  
void write_line(void)
{  
      int page, i;
     
    for (page = 0; page < 8; page ++)
   {
       OLEDSetPos(page, 64);
       for(i = 0; i < 1; i++)
       OLED_Write_Dat(0xff);
   }
     printk("yes\n");
     
} 
/*oled 初始化函数*/
void OLEDInit(void)
{
    /*复位*/
    s3c2410_gpio_setpin(S3C2410_GPF(5), 1);  /*gpio子系统, 可以直接控制引脚*/
    msleep(100);
    s3c2410_gpio_setpin(S3C2410_GPF(5), 0);
    msleep(100);
    s3c2410_gpio_setpin(S3C2410_GPF(5), 1);
    
    /* 向OLED发命令以初始化 */
    OLED_Write_Cmd(0xAE); /*display off*/ 

    OLED_Write_Cmd(0x00); /*set lower column address*/ 

    OLED_Write_Cmd(0x10); /*set higher column address*/

    OLED_Write_Cmd(0x40); /*set display start line*/ 

    OLED_Write_Cmd(0xB0); /*set page address*/ 

    
    OLED_Write_Cmd(0x81); /*contract control*/ 

    OLED_Write_Cmd(0xff); /*128*/ 

    OLED_Write_Cmd(0xA1); /*set segment remap*/ 

    OLED_Write_Cmd(0xA6); /*normal / reverse*/ 

    OLED_Write_Cmd(0xA8); /*multiplex ratio*/
    
    OLED_Write_Cmd(0x3F); /*duty = 1/64*/ 

    OLED_Write_Cmd(0xC8); /*Com scan direction*/

    OLED_Write_Cmd(0xD3); /*set display offset*/

    OLED_Write_Cmd(0x00); 

    OLED_Write_Cmd(0xD5); /*set osc division*/
    
    OLED_Write_Cmd(0x80); 
  
    OLED_Write_Cmd(0xD9); /*set pre-charge period*/ 
 
    OLED_Write_Cmd(0x1f); 

    OLED_Write_Cmd(0xDA); /*set COM pins*/

    OLED_Write_Cmd(0x12); 

    OLED_Write_Cmd(0xdb); /*set vcomh*/

    OLED_Write_Cmd(0x30); 

    OLED_Write_Cmd(0x8d); /*set charge pump enable*/ 

    OLED_Write_Cmd(0x14); 
    
    OLEDSetPageAddrMode();//2
      
    OLED_Write_Cmd(0xAF); /*display ON*/
} 

/*write*/
static ssize_t oled_write(struct file *fd, const char __user *buf, size_t count, loff_t *fpos)
{
    int ret;  
    if (count > 4096)
        return -EINVAL;	
        
    ret = copy_from_user(ker_buf, buf, count);
    
    OLED_Set_DC(1);
    spi_write(oled_spi_dev, ker_buf, count); /*通过spi子系统的接口,向oled发送数据*/
       
    return 0;
}

/*字符设备开打函数*/
static int oled_open(struct inode *node, struct file *fd)
{
    //OLEDInit();  
    printk(KERN_WARNING"oled is open..\n");      
    return 0;	
}

/*ioctl*/
static long oled_ioctl(struct file *fd, unsigned int cmd, unsigned long arg)
{ 
    int page;
    int col;
    
    switch(cmd)
    {
        case OLED_CMD_INIT:  /*初始化*/
            printk(KERN_WARNING"oled is initing..\n");
            OLEDInit();
            break;
         
        case OLED_CMD_CLEAR_ALL: /*清全屏*/
            printk(KERN_WARNING"oled is clearing..\n"); 
            OLEDClear();
            break;
                
        case OLED_CMD_SET_POS:  /*设置坐标位置,高8位为横坐标, 第八位为页地址*/          
            page = arg & 0xff;
            col  = (arg >> 8) & 0xff;
            OLEDSetPos(page, col);            
            break;
            
        case OLED_CMD_CLEAR_PAGE: /*清除页地址*/  
            page = arg;
            OLEDClearPage(page);
            break;
              	
    }
    
    return 0;    	
}


/*字符设备操作函数集*/
static struct file_operations oled_fops = 
{
    .owner            = THIS_MODULE,
    .unlocked_ioctl   = oled_ioctl,
    .write            = oled_write,
    .open             = oled_open,
};

/*oled_spi_probe函数*/
static int oled_spi_probe(struct spi_device *spi)  
{
   // int ret;
    oled_spi_dev = spi; /*通过全局变量过去spi_device*/
    
    ker_buf = kmalloc(4096, GFP_KERNEL);
    if (!ker_buf) 
    {
        printk(KERN_WARNING"get the malloc fail!\n");
	goto err0;
    }

    /*获取dc引脚, 并设置为输出*/
    oled_spi_dc_pin = (int)spi->dev.platform_data; /*获取 数据/命令 控制引脚*/
    s3c2410_gpio_cfgpin(oled_spi_dc_pin, S3C2410_GPIO_OUTPUT); /*设置为输出*/
    s3c2410_gpio_cfgpin(spi->chip_select, S3C2410_GPIO_OUTPUT);
    
        
    if((major = register_chrdev(0, CDEV_NAME, &oled_fops)) < 0) /*注册字符设备,第一个参数为0, 系统为驱动自动分配一个主设备号*/
    {
        printk(KERN_ERR "add the chrdev fail!\n");
        goto err1;   	
    }
    
    oled_class = class_create(THIS_MODULE, CDEV_NAME); /*创建类*/
    if(IS_ERR(oled_class))
    {
         printk(KERN_WARNING"create the class fail!\n");
         goto err2; 
    }
    
    device_create(oled_class, NULL, MKDEV(major, 0), NULL, CDEV_NAME);  /*创建文件设备节点,第二个参数为设备号, 通过主设备号和从设备号 生成*/ 
    
    return 0;
    
    err2:  
        class_destroy(oled_class);
    err1:
        unregister_chrdev(major, CDEV_NAME);
    err0:
        kfree(ker_buf); 
        
        return -1; 
              	
}


static int oled_spi_remove(struct spi_device *spi)
{
     device_destroy(oled_class, MKDEV(major, 0));
     class_destroy(oled_class);
     unregister_chrdev(major, CDEV_NAME);
     kfree(ker_buf); 
     
     return 0;  	
}

/*spi_driver*/
static struct spi_driver spi_oled_driver = 
{
    .driver = {
        .name = "oled",	
        .owner = THIS_MODULE,
    },
    .probe = oled_spi_probe,
    .remove =  __devexit_p(oled_spi_remove), /*只有注册了设备,remove函数才会生效*/
};


/*oled_init*/
static int __init oled_init(void)
{
    return spi_register_driver(&spi_oled_driver); 	/*注册驱动到spi总线*/
}


/*oled_exit*/
static void __exit oled_exit(void)
{
    spi_unregister_driver(&spi_oled_driver);  /*注销oled_spi 驱动*/   	
}


module_init(oled_init);
module_exit(oled_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("WUHAO");
MODULE_DESCRIPTION("OLED SPI driver");