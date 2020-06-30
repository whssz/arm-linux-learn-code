#include <linux/module.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>
#include <linux/slab.h>
#include "memdev.h"

unsigned char mem1[10] = {40,20,15,5,5,7,7,1,1,8};


struct cdev mem_dev; //1.1静态分配cdev结构
dev_t memdev_num = 0; //主设备号

/*mem_open*/
static int mem_open(struct inode *inode, struct file *filp)
{
    int num = MINOR(inode->i_rdev);
    
    if(0 == num)
    {
         //2.1 获取设备的寄存器地址
         filp->private_data = mem1;     
    }
    else
    {
        return -ENODEV;
    } 
    
   return 0;  	
}
/*mem_read*/
static ssize_t mem_read(struct file *filp, char __user *buf, size_t size, loff_t *f_pos)
{   
    unsigned char *register_addr; 
    unsigned int count = size; 
    loff_t p = *f_pos;  //文件指针内存储的是相对于文件的字节偏移量
    unsigned int ret = 0;
     
    //判断读写位置是否有效
    if(p >= 10*sizeof(char)) 
        return 0;
    if((p + count)>10*sizeof(char))
       count = 10*sizeof(char) - p;
       
    //1.从file中拿到基地址
    register_addr = (char *)filp->private_data;
    
    //2.把读到的数据返回给应用程序    
    if(copy_to_user(buf, register_addr+p, count))  //指针+num 是对指向的类型数相加.!!!!!!!!!!!!!!
    {  
    	ret = -EFAULT;
        return ret;
    }
    else
    {
        //3.file里面的文件指针偏移
        *f_pos +=count; 
        ret = count; 	
    }
      
    //返回读出个数
    return ret;
}

/*mem_wriet*/
static ssize_t mem_write(struct file *filp, const char __user *buf, size_t size, loff_t *f_pos)
{    
     unsigned int *register_addr;
     unsigned int count = size; 
     loff_t p = *f_pos;  //文件指针内存储的是相对于文件的字节偏移量
     unsigned int ret = 0;
     
     //判断读写位置是否有效
     if(p >= 10*sizeof(char)) 
        return 0;
     if((p + count)>10*sizeof(char))
        count = 10*sizeof(char) - p;
        
     //1.从file中拿到基地址
      register_addr = filp->private_data;
      
     //2.把数据写入内核   
     if(copy_from_user(register_addr+p, buf, count))
     {  
    	 ret = -EFAULT;
         return ret;  //发生错误还是返回写入个数
     }
     else
     {
         //3.file里面的文件指针偏移
         *f_pos +=count; 
          ret = count; 	
     }  
         
     //4.返回写入个数   
     return ret; 	
}

/*mem_llseek*/
static loff_t mem_llseek(struct file *filp, loff_t offset, int whence)
{    
     loff_t new_pos;
     switch(whence) 
     {
         case SEEK_SET:
             new_pos = offset;
             break;
             
         case SEEK_CUR:
             new_pos = filp->f_pos + offset;
             break;
             
         case SEEK_END:	
             new_pos = (10*sizeof(char)-1) + offset;
             break;
                        
         default:
             return -ENOENT;  
     }
     if(new_pos > (10*sizeof(char)) || new_pos < 0) 
         return -EFAULT; //错误地址
         
     filp->f_pos = new_pos;
     
     return new_pos;   	
}

/*mem_ioctl*/
static long mem_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{  
    
    switch(cmd)
    {
        case MEM_SET:
        printk(KERN_WARNING"the arg is %ld", arg);
        return 0;
        
        case MEM_RESTART:
        printk(KERN_WARNING"the device is restart");
        return 0;
        
        default:
        return -EINVAL;
    }	  
}

/*mem_close*/
static int mem_close(struct inode *inode, struct file *filp)
{
   //关闭硬件设备
   return 0;	
}


/*file_operation*/
const struct file_operations mem_fops= 
{
    .llseek = mem_llseek,
    .open = mem_open,
    .write = mem_write,
    .read = mem_read, 
    .release = mem_close,
    .unlocked_ioctl = mem_ioctl,
};

/*module_in  function*/
static int __init mem_init(void)
{   
    int ret;
    //1.2 初始化cdev
     cdev_init(&mem_dev, &mem_fops);
     
    //1.3 注册cdev
    
    //1.3.1动态分配主设备号
     ret = alloc_chrdev_region(&memdev_num, 0, 1, "memdev");
     if(ret)
         goto err_dev;
    //1.3.2 注册, 其实也是填充 cdev 结构   
     ret = cdev_add(&mem_dev, memdev_num, 1);
     if(ret < 0)
     {   
     	 printk(KERN_WARNING"add dev fial!\n");
         goto err_add;
     }
     printk(KERN_WARNING"Im here!\n");
     return 0;	
    
     err_add:
         cdev_del(&mem_dev);   
     err_dev:
         unregister_chrdev_region(memdev_num, 1);
     return ret;      	
}

/*module_out function*/
static void __exit mem_exit(void)
{  
   //1. 注销cdev结构体	
   cdev_del(&mem_dev);
   
   //2. 注销主设备号 
   unregister_chrdev_region(memdev_num, 1);
}

module_init(mem_init);
module_exit(mem_exit);

MODULE_LICENSE("GPL");