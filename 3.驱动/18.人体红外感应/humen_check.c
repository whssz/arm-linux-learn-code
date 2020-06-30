#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/io.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>
#include <linux/timer.h>
#include <linux/delay.h>
#include <linux/sysctl.h>
#include <asm/uaccess.h>
#include "humen_check.h"

#define GPBCON  0x56000010
#define GPBDAT  0x56000014
#define GPFCON  0x56000050
#define GPFDAT  0x56000054

unsigned int  *gpb_con;
unsigned int  *gpb_data;
unsigned int  *gpf_con;
unsigned int  *gpf_data;

static struct work_struct *work1;
char irq_flag=0;


/*����Ҫ�ύ����,�˺�����Ч*/
static void work1_func(struct work_struct *work)
{   
     unsigned int val=0;
     
     msleep(1000);
     
     val = readl(gpb_data);
     val &= (~0x1);	
     writel(val, gpb_data);
}

/*irq_handle*/
static irqreturn_t humck_handle(int irq, void *handle)
{
   
    /*�����Ƿ�����ж�*/
    
    /*����ж�,cpu�Զ����*/
    unsigned int val=0;
    
   // val = readl(gpb_data);
    //val |= 0x1; 
    //writel(val, gpb_data);
     
    //schedule_work(work1);
    irq_flag = 1; /*��λ�жϲ�����־*/ 
     
    return IRQ_HANDLED;    	
}

/*���豸*/
static int humck_open(struct inode *node, struct file *filp)
{   
    unsigned int val=0;
    int ret;
    
    /*2д��������Ĵ���ֵ*/ 
    val = readl(gpb_con);
    val &= ~(3<<0);  
    val |= 0b1; 
    writel(val, gpb_con);  
   
    val = readl(gpb_data);
    val &= (~0x1);
    writel(val, gpb_data);
    
    
    /*4. д�����Ĵ���*/
    val = readl(gpf_con);
    val &= ~(3<<2);
    val |=  (0x2<<2);
    writel(val, gpf_con);
    
    val = readl(gpf_data);
    val &= ~(0x1<<1);  
    writel(val, gpf_data);  
    
    
      /*2.ע���ж�*/
    ret = request_irq(IRQ_EINT1, humck_handle, IRQF_TRIGGER_RISING, "humen_check", 0);
    if(ret)
    {
        printk(KERN_WARNING"add irq1 fial!\n");
        return ret;
    }  
            
    return 0;            	
}

static ssize_t humck_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{ 

    if(copy_to_user(buf, &irq_flag, count))     
         return -EFAULT; 
       
    return 0;	
}

static ssize_t  humck_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
   
    if(copy_from_user(&irq_flag, buf, count))
        return -EFAULT; 
        
    return 0;  
    	
}

static int humck_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
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
        
        default:
        printk(KERN_WARNING"please put right cmd!\n");
        break;        
    }
    return 0;       
}

static int humck_close(struct inode *node, struct file *filp)
{     
    unsigned int val =0;
    /*�رշ�����*/
    val = readl(gpb_data);
    val &= (~0x1);	
    writel(val, gpb_data); 
    
    return 0;    	
}

/*��ʼ��file_operation*/
struct file_operations check_fops=
{
    .open = humck_open,
    .read = humck_read,
    .write = humck_write,
    .release = humck_close,
    .unlocked_ioctl = humck_ioctl,
    	
};


/*1.��̬����miscdevice�ṹ*/
struct miscdevice humck_miscdev= 
{
    .minor = 201,
    .name = "humen_check",  //�豸�ڵ��� "/dev/"
    .fops = &check_fops,	
};


static int __init hm_check_init(void)
{
     int ret;
 
     /*1.ע��misc�豸����*/
     ret =  misc_register(&humck_miscdev);
     if(ret < 0)
     {   
     	 printk(KERN_WARNING"add miscdev fail!\n");
         goto err_add;
     }
     
     /*1.��������ȡ�Ĵ���ӳ���ַ*/
     gpb_con = ioremap(GPBCON, 4);
     if(!gpb_con)
     {
         printk(KERN_WARNING"gpb_con ioremap fail\n");
         goto err_map;	
     }
    
     gpb_data = ioremap(GPBDAT, 4);
     if(!gpb_data)
     {
         printk(KERN_WARNING"gpb_data ioremap fail\n");
         goto err_map;	
     }
    
      /*3.��ȡ����Ĵ�����ֵ*/
     gpf_con = ioremap(GPFCON, 4);
     if(!gpf_con)
     {
        printk(KERN_WARNING"gpf_con ioremap fail\n");
        goto err_map;	
     }
    
     gpf_data = ioremap(GPFDAT, 4);
     if(!gpf_data)
     {
         printk(KERN_WARNING"gpf_data ioremap fail\n");
         goto err_map;	
     }
      /*4.ע�Ṥ������*/
     //work1 = kmalloc(sizeof(struct work_struct), GFP_KERNEL);
     //INIT_WORK(work1, work1_func);
    
    return 0;
    	
    err_add : misc_deregister(&humck_miscdev);   
           
    err_map :
    	    return ret;
    	   
}
 
 

static void __exit hm_check_exit(void)
{    
   free_irq(IRQ_EINT1, 0);
   //kfree(work1);
   misc_deregister(&humck_miscdev);   
    
}

module_init(hm_check_init);
module_exit(hm_check_exit);

MODULE_LICENSE("GPL");