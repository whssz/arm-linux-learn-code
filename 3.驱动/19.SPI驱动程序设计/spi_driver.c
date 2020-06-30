#include <linux/init.h>
#include <linux/module.h>

#define spi_name "mini2440_spi"
static int spi_major = 55;
struct cdev spi_cdev;

static int __init myspi_init(void)
{ 
    int ret;
    
    /*1.分配设备*/
    dev_t devno = MKDEV(spi_major, 0); 
    /*2.注册设备*/
    if(spi_major) 
        ret = register_chrdev_region(devno, 1, spi_name);
    else
    {
        ret = alloce_chrdev_region(&devno, 0, 1, spi_name);
        if(ret < 0)
            return ret;
            
        spi_major = MAJOR(devno);/*获取主设备号*/        
    } 
     
    cdev_init(&spi_cdev, &spi_fops);
    spi_cdev.ower = THIS_MODULE;
    spi_cdev.ops = &spi_fops;
    cdev_add(&spi_cdev, devno, 1);
          	
}

static void __exit myspi_exit(void)
{
   	
}

module_init(myspi_init);
module_exit(myspi_exit);
MODULE_LICENSE("GPL");