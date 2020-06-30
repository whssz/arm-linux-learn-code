#include  <linux/init.h>
#include  <linux/module.h>
#include  <linux/types.h>
#include  <linux/delay.h>
#include  <linux/fs.h>
#include  <linux/cdev.h>
#include  <linux/device.h>
#include  <linux/io.h>
#include  <asm/uaccess.h> 
#include  <linux/kernel.h>
#include  <linux/miscdevice.h>
#include  "adc_h.h"

#define ADCCON   0x58000000  //ADC control
#define ADCTSC   0x58000004   //ADC touch screen contro
#define ADCDAT0  0x5800000C 
#define GPBCON   0x56000010  //beep 
#define GPBDAT   0x56000014

unsigned int  *adc_con;
unsigned int  *adc_data;
unsigned int  *adc_tsc;
unsigned int  *gpb_con;
unsigned int  *gpb_data;

#define CDEV_NAME   "adc"

/*åŠ¨æ€åˆ†é…ä½è®¾å¤‡å·ï¼Œéœ€è¦ç”¨åˆ°çš„å‚æ•°*/
dev_t dev = 0;
struct cdev dev_c;
/*åŠ¨æ€åˆ†é…ç±»*/
struct class *adc_class;

/*open*/
static int adc_open(struct inode *inode, struct file *fd)
{   
    unsigned int value=0; 
    
    /*Ó³ÉäµØÖ·,³õÊ¼»¯ADCCONºÍADCTSC*/
    adc_con = ioremap(ADCCON, 4);
    if(!adc_con)
    {
        printk(KERN_WARNING"adc_con ioremap fail\n");
        goto err;	
    }
    /*ÉèÖÃadcÊ±ÖÓÆµÂÊ*/
    value = readl(adc_con);
    value |= (1<<14) | (49<<6); 
    value &= ~(0x7<<3);   
     
    writel(value, adc_con);
    value = 0;
    
    /*ÉèÖÃadcÎªÆÕÍ¨ad×ª»»Ä£Ê½*/
    adc_tsc = ioremap(ADCTSC, 4);
    if(!adc_tsc)
    {
        printk(KERN_WARNING"adc_tsc ioremap fail\n");
        goto err;	
    }
    writel(0x0, adc_tsc);
    
    
    adc_data = ioremap(ADCDAT0, 4);
    if(!adc_data)
    {
        printk(KERN_WARNING"adc_data ioremap fail\n");
        goto err;	
    }
    
    fd->private_data = adc_con; //³¢ÊÔĞÔ±£´æÒ»¸öadc_con

    return 0;
    
    err:
        return -1;
}

/*write*/
static ssize_t adc_write(struct file *fd, const char __user *buffer, size_t count, loff_t *f_pos)
{  
    unsigned int channel=0;
    unsigned int value=0;
    
    int *register_addr = fd->private_data;
    
    if(copy_from_user(&channel, buffer, count))  //»ñÈ¡Í¨µÀ
    {
        printk(KERN_WARNING"write from user fail!\n");	  
    }
    /*ÏÈ°ÑÍ¨µÀÇå0*/
    value = readl(register_addr); 
    value &= ~(0x7<<3);   
    value |= (channel<<3);
       
    writel(value, register_addr);  
   
    return count;
}


/*read*/
static ssize_t adc_read(struct file *fd, char __user *buf, size_t count, loff_t *f_pos)
{ 
    unsigned int value=0;
    unsigned short ad_data=0;
    
    value = readl(adc_con);
    value |= 0x1; //¿ªÆô×ª»»
    writel(value, adc_con);
    
    while(readl(adc_con) & 0x1);//µÈ´ı´«Êä¿ªÊ¼
  
    while(!(readl(adc_con) & (1<<15))); //µÈ´ı´«Êä½áÊø
  
    ad_data = (readl(adc_data) & 0x3ff);

    copy_to_user(buf, &ad_data, count); 
    
    return count;  
}

/*close*/
static int adc_close(struct inode *inode, struct file *fd) 
{  
    printk(KERN_WARNING"adc is closing!\n");
    return 0;
}

/*ioctl*/
static int adc_ioctl(struct file *fd, unsigned int cmd, unsigned long arg)
{ 
    unsigned int val = 0;
    
    switch(cmd)
    {
        case BEEP_ON:       
        val = readl(gpb_data);
        val |= 0x1; 
        writel(val, gpb_data);
        return 0;
        
        case BEEP_OFF:
        val = readl(gpb_data);
        val &= (~0x1);	
        writel(val, gpb_data); 
        return 0;
        
        case LED_ON:
        val = readl(gpb_data);
        val &= ~(0xf<<5);		
        writel(val, gpb_data);
        return 0;
        
        case LED_OFF:
        val = readl(gpb_data);
        val |= (0xf<<5);		
        writel(val, gpb_data);
        return 0;
        
        default:
        printk(KERN_WARNING"please put right cmd!\n");
        break;        
    }
    return 0;       
}

/*å­—ç¬¦è®¾å¤‡æ“ä½œå‡½æ•°é›†*/
static struct file_operations adc_fops=
{
    .owner = THIS_MODULE,
    .read = adc_read,
    .write = adc_write,
    .release = adc_close,
    .unlocked_ioctl = adc_ioctl,
    .open = adc_open,
};

/*Èë¿Úº¯Êı*/
static int __init adc_init(void)
{  
     int ret=0;
     
    /*æ³¨å†Œå­—ç¬¦è®¾å¤‡*/
      //1.åŠ¨æ€åˆ†é…ä¸»è®¾å¤‡å·
     
     ret = alloc_chrdev_region( &dev, 0, 1, CDEV_NAME );
     if(ret)
     {
         printk(KERN_WARNING"get the cdev major fail!\n");
         goto err1;
     } 
     
     cdev_init( &dev_c, &adc_fops );
     
     ret = cdev_add( &dev_c, dev, 1);  
     if(ret)
     {
          printk(KERN_WARNING"add the cdev  fail!\n"); 
          goto err1; 
     }
     
     
    /*æ³¨å†Œä¸€ä¸ªclass*/
    adc_class = class_create( THIS_MODULE, CDEV_NAME );
    if(IS_ERR(adc_class ))
    {
         printk(KERN_WARNING"create the class fail!\n");
         goto err2; 
    }
       
    /*åˆ›å»ºè®¾å¤‡æ–‡ä»¶èŠ‚ç‚¹*/
    device_create(adc_class, NULL, dev, NULL, CDEV_NAME);
   
   /*led ºÍ beep µÄÓ²¼ş³õÊ¼»¯*/
    if(led_beep_hardinit())
        goto err0;
    
    return 0;
     
    err2:
        device_destroy(adc_class, dev); 
        class_destroy(adc_class);
    err1:
        unregister_chrdev_region(dev, 1);  /*arg1ï¼š è®¾å¤‡å¥½  arg2ï¼šè®¾å¤‡ä¸ªæ•°*/
    err0:
        return -1;
 
}

/*ÍË³öº¯Êı*/
static void __exit adc_exit(void)
{
	device_destroy(adc_class, dev); 
	class_destroy(adc_class);
	unregister_chrdev_region(dev, 1);
}



/*LED BEEP Ó²¼ş³õÊ¼»¯º¯Êı*/
int led_beep_hardinit(void)
{ 
    unsigned int value = 0;
    gpb_con = ioremap(GPBCON, 4);  /*Ó³ÉäĞéÄâµØÖ·*/
    if(!gpb_con)
    {
        printk(KERN_WARNING"gpb_con ioremap fail\n");
        goto err;	
    }
    
    gpb_data = ioremap(GPBDAT, 4);
    if(!gpb_data)
    {
        printk(KERN_WARNING"gpb_data ioremap fail\n");
        goto err;	
    }
    
    /*2.³õÊ¼»¯·äÃùÆ÷¼Ä´æÆ÷Öµ,beepÃğ*/ 
    value = readl(gpb_con);
    value &= ~(3<<0);  
    value |= 0x1; 
    writel(value, gpb_con);  
    value = 0;
    
    value = readl(gpb_data);
    value &= (~0x1);
    writel(value, gpb_data);
    value = 0;
    
    /*3.³õÊ¼»¯LEDµÆ,ledµÆÃğ*/
    value = readl(gpb_con);
    value &= ~(0xff<<10);
    value |=(0x55<<10);
    writel(value, gpb_con);
    value = 0;
    
    value = readl(gpb_data);
    value |= (0xf<<5);	
    writel(value, gpb_data);
    value = 0;
    
    return 0;
    
    err:
        return -1;
}


module_init(adc_init);
module_exit(adc_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("WUHAO");
MODULE_DESCRIPTION("ADC collect");

