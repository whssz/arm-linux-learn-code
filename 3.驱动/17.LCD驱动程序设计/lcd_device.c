#include <linux/module.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/platform_device.h>
#include <linux/kthread.h>
#include <linux/slab.h>
#include <linux/dma-mapping.h>
#include <asm/dma.h>
#include <mach/hardware.h>
#include <mach/dma.h>

#define GPCCON     0x56000020
#define GPDCON     0x56000030
#define GPGCON     0x56000060

#define LCDCON1    0x4D000000
#define LCDCON2    0x4D000004
#define LCDCON3    0x4D000008
#define LCDCON4    0x4D00000C
#define LCDCON5    0x4D000010
#define LCDSADDR1  0x4D000014
#define LCDSADDR2  0x4D000018
#define LCDSADDR3  0x4D00001C

#define TPAL       0x4D000050

unsigned short LCDBUFFER[320][240];


struct resource lcd_resource[]=
{
    [0] = {
            .start = GPCCON,
            .end = GPCCON + 44,
            .flags = IORESOURCE_MEM,        	
    }, 

    [1] = {
    	    .start = LCDCON1,
    	    .end = LCDCON1 + 32,
    	   .flags = IORESOURCE_MEM,    	    
    },
    [2] = {
    	   .start = TPAL,
    	   .end = TPAL+4,
    	   .flags = IORESOURCE_MEM,
    }, 	   
     	
};


static unsigned int lcd_dmamask = 0xffffffff;
struct platform_device lcd_dev=
{
    .name = "my_lcd",
    .id = 0,
    .num_resources = 3,
    .resource = lcd_resource, 
    .dev = {
    	    .dma_mask = &lcd_dmamask,
            .coherent_dma_mask = 0xffffffff,          	
    },   	
};

static int __init lcddev_init(void)
{    
    platform_device_register(&lcd_dev);  
       
    return 0;		
}

static void __exit lcddrv_exit(void)
{
    platform_device_unregister(&lcd_dev);		
}


module_init(lcddev_init);
module_exit(lcddrv_exit);

MODULE_LICENSE("GPL");