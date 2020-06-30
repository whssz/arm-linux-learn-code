#include <linux/module.h>
#include <linux/compat.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/major.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/mman.h>
#include <linux/vt.h>
#include <linux/init.h>
#include <linux/linux_logo.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/console.h>
#include <linux/kmod.h>
#include <linux/err.h>
#include <linux/device.h>
#include <linux/efi.h>
#include <linux/fb.h>
#include <linux/dma-mapping.h>
#include <linux/platform_device.h>
#include <asm/mach/map.h>
#include <asm/io.h>
#include <mach/regs-lcd.h>
#include <mach/regs-gpio.h>

struct resource *lcd_resource1;
struct resource *lcd_resource2;
struct resource *lcd_resource3;
unsigned int *resource1_base;
unsigned int *resource2_base;
unsigned int *resource3_base;
char __iomem *fb_mem;
struct cdev lcd_dev; //1.1静态分配cdev结构

#define CLKVAL 4   //10 = 100/((CLKVAL+1)*2)
#define VSPW 9
#define VBPD 0
#define VFPD 0
#define HSPW 4
#define HBPD 100
#define HFPD 0
#define LINEVAL 319
#define HOZVAL 239

/*lcd _open*/
static int lcd_open(struct inode *inode, struct file *filp)
{  
     unsigned int val=0;     
   
     val = readl(resource1_base + 10);
     val |= (0x3<<8);
     writel(val , resource1_base + 10);
       
     val = readl(resource2_base + 4);
     val |=(1<<3);
     writel(val , resource2_base + 4);
     
     val = readl(resource2_base);
     val |= 1;
     writel(val , resource2_base);
     
     printk(KERN_WARNING"we had open the lcd!\n");     
     val = 0;
     
     return 0;
}

/*操作函数集*/
static const struct file_operations lcd_fops = {
	.owner =	THIS_MODULE,
	//.read =		lcd_read,
	//.write =	lcd_write,
	.open = 	lcd_open,
	//.unlocked_ioctl = lcd_ioctl,
};

/*找到设备执行函数*/
static int lcd_probe(struct platform_device *pdev)
{
      int ret = 0;
      unsigned int val=0;
      dma_addr_t map_dma;
      unsigned long saddr1, saddr2, saddr3;
      
      /*1.获取资源*/
      lcd_resource1 = platform_get_resource(pdev, IORESOURCE_MEM, 0);
      if(!lcd_resource1)
      {
          printk(KERN_WARNING"get the resource fail!\n");
          return -ENODEV;        
      }
      
      lcd_resource2 = platform_get_resource(pdev, IORESOURCE_MEM, 1);
      if(!lcd_resource2)
      {
          printk(KERN_WARNING"get the resource fail!\n");
          return -ENODEV;        
      }
      
      lcd_resource3 = platform_get_resource(pdev, IORESOURCE_MEM, 2);
      if(!lcd_resource3)
      {
          printk(KERN_WARNING"get the resource fail!\n");
          return -ENODEV;        
      }
      
      /*2.转化地址*/
      resource1_base = ioremap(lcd_resource1->start, 44);
      if(!resource1_base)
      {
         printk(KERN_WARNING"map the register_base fail!\n");
         return -EFAULT;          	
      }
      
      resource2_base = ioremap(lcd_resource2->start, 32);
      if(!resource2_base)
      {
         printk(KERN_WARNING"map the register_base fail!\n");
         return -EFAULT;          	
      }
      
      resource3_base = ioremap(lcd_resource3->start, 4);
      if(!resource3_base)
      {
         printk(KERN_WARNING"map the register_base fail!\n");
         return -EFAULT;          	
      }
      
      /*3.disable_lcd*/
      val = readl(resource2_base);
      val = val & (~(1<<0));
      writel(val , resource2_base);
      
      //int x = dma_set_mask(pdev, 0xffffff);
      /*分配帧缓冲地址*/
      unsigned map_size = PAGE_ALIGN(320*240*2);

      fb_mem =  dma_alloc_writecombine(pdev, map_size, &map_dma, GFP_KERNEL);
      if(fb_mem == NULL)
      {
          printk(KERN_WARNING"get the dma_add fail!\n");
          return -EFAULT;    	
      else
      {
           memset(fb_mem, 0x00, map_size);   	
      }
      
      /*4.引脚初始化*/
      writel(0xaaaaaaaa, resource1_base);
      writel(0xaaaaaaaa, resource1_base+4);
      
      /*5.LCD控制器初始化*/
      /*LCDCON1 = (CLKVAL<<8)|(0x3<<5)|(0xc<<1)|(0<<0);  //设置时钟
      LCDCON2 = (VBPD<<24)|(LINEVAL<<14)|(VFPD<<6)|(VSPW);
      LCDCON3 = (HBPD<<19)|(HOZVAL<<8)|(HFPD);
      LCDCON4 = (HSPW);
      LCDCON5 = (1<<11)|(1<<9)|(1<<8);
      */
      writel(((CLKVAL<<8)|(0x3<<5)|(0xc<<1)|(0<<0)),  resource2_base);
      writel(((VBPD<<24)|(LINEVAL<<14)|(VFPD<<6)|(VSPW)),  resource2_base+1);
      writel(((HBPD<<19)|(HOZVAL<<8)|(HFPD)),  resource2_base+2);
      writel((HSPW),  resource2_base+3);
      writel(((1<<11)|(1<<9)|(1<<8)),  resource2_base+4);
      
      /*5.写入地址*/
      saddr1 = ((map_dma>>22)<<21) | ((map_dma>>1)&0x1fffff);
      saddr2 = ((map_dma + 320*240*2)>>1)&0x1fffff;
      saddr3 = (0<<11) | (240*2/2);
    	
      writel(saddr1,  resource2_base+5); 
      writel(saddr2,  resource2_base+6);
      writel(saddr3,  resource2_base+7);
      
      /*6. 关闭调色板*/
      writel(0, resource3_base);
      
      /*7.注册字符设备*/
      register_chrdev(29, "fb", &lcd_fops);
      ret = cdev_add(&lcd_dev, 29, 1);
      if(ret < 0)
      {   
     	  printk(KERN_WARNING"add dev fial!\n");
          return ret;
      }
  
      return 0;				       
}
/*移除函数*/
static int lcd_remove(struct platform_device *pdev)
{
     cdev_del(&lcd_dev);
     return 0;	
}

/*驱动结构体*/
static struct platform_driver lcd_driver=
{
    .probe = lcd_probe,
    .remove = lcd_remove,
    .driver ={
           .name = "my_lcd",
           .owner = THIS_MODULE,  	
     },
};

/*入口函数*/
static int __init lcd_init(void)
{ 
    int ret = platform_driver_register(&lcd_driver);  /*注册平台驱动设备*/
    if(ret)
    {
        printk(KERN_WARNING"add platform lcd fail!\n");
        return ret;
    }

    return 0;	
}

/*出口函数*/
static void __exit lcd_exit(void)
{
    platform_driver_unregister(&lcd_driver);   	
}


module_init(lcd_init);
module_exit(lcd_exit);

MODULE_LICENSE("GPL");