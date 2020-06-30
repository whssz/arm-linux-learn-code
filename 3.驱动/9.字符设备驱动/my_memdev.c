#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>

struct cdev mdev; //静态分配cdev
dev_t deno;
int register1[5];
int register2[5];
 


static loff_t mem_llseek(struct file *file, loff_t offset, int whence)
{ 
   loff_t newpos;
   
   switch (whence){
   	 case SEEK_SET :
   	   newpos =  offset;
   	   break;
   	   
   	 case SEEK_CUR :
   	   newpos = file->f_pos + offset;
   	   break; 
   	   
   	 case SEEK_END :   	 	
   	  newpos = 5*sizeof(int)-1 + offset;
   	  break;
   	  
   	  default: 
   	  break;
   	 }
   	 
   	if ((newpos<0) || (newpos>5*sizeof(int)))
    	    return -EINVAL; //返回-1
    	    
   	file->f_pos = newpos;
   	    return newpos;   	  
} 


static ssize_t mem_read(struct file *file, char __user *buffer, size_t size, loff_t *f_pos)
{    
    unsigned long p = *f_pos;
    unsigned int count = size;
    int *register_addr =  file->private_data ;
    
    if(p > 5*sizeof(int))
        return 0;
    if(count > (5*sizeof(int)- p))
        count = 5*sizeof(int)- p;
    
    copy_to_user(buffer, register_addr+p, count);
    
    *f_pos += count; 
    return count;
      		
}

static ssize_t mem_write(struct file *file, const char __user *buffer, size_t size, loff_t *f_pos)
{
    unsigned long p = *f_pos;
    unsigned int count = size;
    int *register_addr =  file->private_data ;
    
    copy_from_user(register_addr+p, buffer, count);
    
    *f_pos += count;  //传入的指针,所以可以改变里面的值
    return count;
}

int mem_open(struct inode *inode, struct file *file)
{
   /*获取次设备号*/
    int num = MINOR(inode->i_rdev);
    
   /*根据次设备号获取设备基地址*/
   if(0 == num)
   {
      file->private_data = register1;
   }
   if(1 == num)
   {
      file->private_data = register2;
   }
   
   return 0;
}

int mem_release(struct inode *inode, struct file *file)
{
    return 0;	
}

static const struct file_operations mem_fops=
{
   .llseek = mem_llseek,
   .read = mem_read,
   .write = mem_write,
   .open = mem_open,
   .release= mem_release,	
};

static int memdev_init(void)
{  
    /*分配cdev*/
    
    /*初始化cdev*/
    cdev_init(&mdev, &mem_fops);
     
    /*注册cdev*/
    alloc_chrdev_region(&deno, 0, 2, "memdev"); //动态分配主设备号
    
    cdev_add(&mdev, deno, 2);
     
}

static void memdev_exit(void)
{
   /*注销cdev*/
   cdev_del(&mdev);
   
   /*释放主设备号*/	
    unregister_chrdev_region(deno, 2);  
}

MODULE_LICENSE("GPL");

module_init(memdev_init);
module_exit(memdev_exit); 