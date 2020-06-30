/**************************************************
*�ļ���: oled_spi_driver.c
*��������:2016.11.20
*�ļ�����: 0.96��oled(128x64)��spi����, 
           linux�汾 : 2.6.32,
           oled����оƬ:SSD1306
           oled����: �����Ϊ8ҳ,�����Ϊ128��,ÿһ���ڵ�ǰҳ��������Ϊ1��byte, �ֽڵ�ÿһbit���������ϵ�1������(����).
           ����: oled��Ҫdc���ſ���д�� ����/����, ����ڰ弶�ļ���piatform_data�д�����.GPF(3)��Ϊ��������.
           ѧϰ�˶�gpio��ϵͳ��һ����֪ʶ, �ܹ���������ں���ʹ������.
           oled_spi_driver.h : �����ioctl����ͷ�ļ�
*ע��: 
     oled��������Ҳ�Ǻ�ɫ,����ȥ��û��ʼ��һ��,�û�����д�����ݹ�����ܿ�������
     Ҳ�����޸������� OLEDClear() ������ OLED_Write_Dat(0);��0�ĵ�255, �ܿ���������ʾ��ɫ.
     Ҳ���� ��ѹ����� ����оƬ 28pin(vcc)  30pin(GND)�ĵ�ѹ, ��ʼ��ʧ��: 2.9v����. ��ʼ���ɹ�: 7.5V���� 
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

/*dc �������ú���*/
static void OLED_Set_DC(char val)
{
     gpio_set_value(oled_spi_dc_pin, val); 	
}

/*oled д�������*/
static void OLED_Write_Cmd(unsigned char cmd)
{
    OLED_Set_DC(0); /* command */
    spi_write(oled_spi_dev, &cmd, 1);
    OLED_Set_DC(1);
}

/*oled д�����ݺ���*/
static void OLED_Write_Dat(unsigned char dat)
{
    OLED_Set_DC(1); /* data */
    spi_write(oled_spi_dev, &dat, 1);
    OLED_Set_DC(1);
}

/*oled �������꺯��*/
static void OLEDSetPos(int page, int col)
{       
    OLED_Write_Cmd(0xB0 + page); /* page address */    
    OLED_Write_Cmd(col & 0xf);   /* Lower Column Start Address */
    OLED_Write_Cmd(0x10 + (col >> 4));   /* Higher Column Start Address */
}

/*oled ������ʼҳ��ַģʽ*/
static void OLEDSetPageAddrMode(void)
{
   OLED_Write_Cmd(0x20);
   OLED_Write_Cmd(0x02);
}


/*oled ��������*/
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

/*oled �������ҳ����*/
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
/*oled ��ʼ������*/
void OLEDInit(void)
{
    /*��λ*/
    s3c2410_gpio_setpin(S3C2410_GPF(5), 1);  /*gpio��ϵͳ, ����ֱ�ӿ�������*/
    msleep(100);
    s3c2410_gpio_setpin(S3C2410_GPF(5), 0);
    msleep(100);
    s3c2410_gpio_setpin(S3C2410_GPF(5), 1);
    
    /* ��OLED�������Գ�ʼ�� */
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
    spi_write(oled_spi_dev, ker_buf, count); /*ͨ��spi��ϵͳ�Ľӿ�,��oled��������*/
       
    return 0;
}

/*�ַ��豸������*/
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
        case OLED_CMD_INIT:  /*��ʼ��*/
            printk(KERN_WARNING"oled is initing..\n");
            OLEDInit();
            break;
         
        case OLED_CMD_CLEAR_ALL: /*��ȫ��*/
            printk(KERN_WARNING"oled is clearing..\n"); 
            OLEDClear();
            break;
                
        case OLED_CMD_SET_POS:  /*��������λ��,��8λΪ������, �ڰ�λΪҳ��ַ*/          
            page = arg & 0xff;
            col  = (arg >> 8) & 0xff;
            OLEDSetPos(page, col);            
            break;
            
        case OLED_CMD_CLEAR_PAGE: /*���ҳ��ַ*/  
            page = arg;
            OLEDClearPage(page);
            break;
              	
    }
    
    return 0;    	
}


/*�ַ��豸����������*/
static struct file_operations oled_fops = 
{
    .owner            = THIS_MODULE,
    .unlocked_ioctl   = oled_ioctl,
    .write            = oled_write,
    .open             = oled_open,
};

/*oled_spi_probe����*/
static int oled_spi_probe(struct spi_device *spi)  
{
   // int ret;
    oled_spi_dev = spi; /*ͨ��ȫ�ֱ�����ȥspi_device*/
    
    ker_buf = kmalloc(4096, GFP_KERNEL);
    if (!ker_buf) 
    {
        printk(KERN_WARNING"get the malloc fail!\n");
	goto err0;
    }

    /*��ȡdc����, ������Ϊ���*/
    oled_spi_dc_pin = (int)spi->dev.platform_data; /*��ȡ ����/���� ��������*/
    s3c2410_gpio_cfgpin(oled_spi_dc_pin, S3C2410_GPIO_OUTPUT); /*����Ϊ���*/
    s3c2410_gpio_cfgpin(spi->chip_select, S3C2410_GPIO_OUTPUT);
    
        
    if((major = register_chrdev(0, CDEV_NAME, &oled_fops)) < 0) /*ע���ַ��豸,��һ������Ϊ0, ϵͳΪ�����Զ�����һ�����豸��*/
    {
        printk(KERN_ERR "add the chrdev fail!\n");
        goto err1;   	
    }
    
    oled_class = class_create(THIS_MODULE, CDEV_NAME); /*������*/
    if(IS_ERR(oled_class))
    {
         printk(KERN_WARNING"create the class fail!\n");
         goto err2; 
    }
    
    device_create(oled_class, NULL, MKDEV(major, 0), NULL, CDEV_NAME);  /*�����ļ��豸�ڵ�,�ڶ�������Ϊ�豸��, ͨ�����豸�źʹ��豸�� ����*/ 
    
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
    .remove =  __devexit_p(oled_spi_remove), /*ֻ��ע�����豸,remove�����Ż���Ч*/
};


/*oled_init*/
static int __init oled_init(void)
{
    return spi_register_driver(&spi_oled_driver); 	/*ע��������spi����*/
}


/*oled_exit*/
static void __exit oled_exit(void)
{
    spi_unregister_driver(&spi_oled_driver);  /*ע��oled_spi ����*/   	
}


module_init(oled_init);
module_exit(oled_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("WUHAO");
MODULE_DESCRIPTION("OLED SPI driver");