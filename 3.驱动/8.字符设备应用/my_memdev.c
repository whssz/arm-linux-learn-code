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


struct cdev mem_dev; //1.1��̬����cdev�ṹ
dev_t memdev_num = 0; //���豸��

/*mem_open*/
static int mem_open(struct inode *inode, struct file *filp)
{
    int num = MINOR(inode->i_rdev);
    
    if(0 == num)
    {
         //2.1 ��ȡ�豸�ļĴ�����ַ
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
    loff_t p = *f_pos;  //�ļ�ָ���ڴ洢����������ļ����ֽ�ƫ����
    unsigned int ret = 0;
     
    //�ж϶�дλ���Ƿ���Ч
    if(p >= 10*sizeof(char)) 
        return 0;
    if((p + count)>10*sizeof(char))
       count = 10*sizeof(char) - p;
       
    //1.��file���õ�����ַ
    register_addr = (char *)filp->private_data;
    
    //2.�Ѷ��������ݷ��ظ�Ӧ�ó���    
    if(copy_to_user(buf, register_addr+p, count))  //ָ��+num �Ƕ�ָ������������.!!!!!!!!!!!!!!
    {  
    	ret = -EFAULT;
        return ret;
    }
    else
    {
        //3.file������ļ�ָ��ƫ��
        *f_pos +=count; 
        ret = count; 	
    }
      
    //���ض�������
    return ret;
}

/*mem_wriet*/
static ssize_t mem_write(struct file *filp, const char __user *buf, size_t size, loff_t *f_pos)
{    
     unsigned int *register_addr;
     unsigned int count = size; 
     loff_t p = *f_pos;  //�ļ�ָ���ڴ洢����������ļ����ֽ�ƫ����
     unsigned int ret = 0;
     
     //�ж϶�дλ���Ƿ���Ч
     if(p >= 10*sizeof(char)) 
        return 0;
     if((p + count)>10*sizeof(char))
        count = 10*sizeof(char) - p;
        
     //1.��file���õ�����ַ
      register_addr = filp->private_data;
      
     //2.������д���ں�   
     if(copy_from_user(register_addr+p, buf, count))
     {  
    	 ret = -EFAULT;
         return ret;  //���������Ƿ���д�����
     }
     else
     {
         //3.file������ļ�ָ��ƫ��
         *f_pos +=count; 
          ret = count; 	
     }  
         
     //4.����д�����   
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
         return -EFAULT; //�����ַ
         
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
   //�ر�Ӳ���豸
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
    //1.2 ��ʼ��cdev
     cdev_init(&mem_dev, &mem_fops);
     
    //1.3 ע��cdev
    
    //1.3.1��̬�������豸��
     ret = alloc_chrdev_region(&memdev_num, 0, 1, "memdev");
     if(ret)
         goto err_dev;
    //1.3.2 ע��, ��ʵҲ����� cdev �ṹ   
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
   //1. ע��cdev�ṹ��	
   cdev_del(&mem_dev);
   
   //2. ע�����豸�� 
   unregister_chrdev_region(memdev_num, 1);
}

module_init(mem_init);
module_exit(mem_exit);

MODULE_LICENSE("GPL");