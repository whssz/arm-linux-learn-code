#include <linux/module.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
#include <linux/kernel.h>
#include <asm/uaccess.h>
#include <linux/fs.h>
#include <linux/io.h>
#include <linux/mutex.h>
#include <linux/wait.h>
#include <linux/kthread.h>

#define GPGCON   0x56000060
#define GPGDAT   0x56000064
#define GPBCON   0x56000010
#define GPBDAT   0x56000014

unsigned int *gpg_con;
unsigned int *gpg_data;
unsigned int *gpb_con;
unsigned int *gpb_data;

unsigned int key_num=0;

struct work_struct *work1;
struct work_struct *work2;

struct timer_list key_timer;

wait_queue_head_t read_wait;

/*key_timer_func*/
static int key_timer_func(void)
{
    unsigned int data=0;
    
    data = readw(gpg_data);
    if(0 == (data&0x01))
    {
        /*�ж�ʵ���ϰ벿,Ӳ������*/
        writel(0x61f, gpb_data);
        key_num = 1;
        
        /*�ж�ʵ���°벿,�ύ����*/
        schedule_work(work1);        	
    }
    
    if(0 == (data&0x80))
    {
        /*�ж�ʵ���ϰ벿,Ӳ������*/
        writel(0x61f, gpb_data);
        key_num = 5;
        
        /*�ж�ʵ���°벿,�ύ����*/
        schedule_work(work1);    	
    }
    
    if(0 == (data&0x08))
    {
        /*�ж�ʵ���ϰ벿,Ӳ������*/
        writel(0x3ff, gpb_data);
        key_num = 2;
        
        /*�ж�ʵ���°벿,�ύ����*/
        schedule_work(work2);   	
    }
    
    /*���ѵȴ�����*/
    wake_up(&read_wait);
    
    return 0;	
}
/*work1_func*/
static int work1_func(void)
{    
    /*�°벿*/
     printk(KERN_WARNING"led on!\n");  
     return 0;  	
}


/*work2_func*/
static int work2_func(void)
{    
    /*�°벿*/
     printk(KERN_WARNING"led off!\n");
     return 0;   	
}


/*irq_handle*/
static irqreturn_t key1_interrpt(int irq, void *handle)
{
    /*�����Ƿ�����ж�*/
    
    /*����ж�,cpu�Զ����*/
    
    /*��������,��ʱ����ʱ*/
    mod_timer(&key_timer, jiffies + HZ/10);
            
    return IRQ_HANDLED;    	
}

static irqreturn_t key2_interrpt(int irq, void *handle)
{
    /*�����Ƿ�����ж�*/
    
    /*����ж�,cpu�Զ����*/
    
    /*��������,��ʱ����ʱ*/
    mod_timer(&key_timer, jiffies + HZ/10);
    
    return IRQ_HANDLED;    	
}

/*key_open*/
static int key_open(struct inode *node, struct file *filp)
{  
   return 0; 	
}

/*key_read*/
static ssize_t key_read(struct file *filp, char __user *buf, size_t size, loff_t *f_ops)
{   
    int ret;
    
    /*�ж��Ƿ����ȴ�����, ��key_numΪ�ж�.*/
    wait_event(read_wait, key_num);
    
    /*��ȡkey_num*/
    ret = copy_to_user(buf, &key_num, 4);
    if(ret < 0)
    {
        ret = -EFAULT;	
        goto err_read;
    }
    
    key_num = 0;
    
    return 4;
    
    err_read:
        return ret;
}

/*led_hardware*/
static int led_hardware_init(void)
{
    gpb_con = ioremap(GPBCON, 4);
    if(!gpb_con)
    {
        printk(KERN_WARNING"led_con ioremap fail\n");
        goto err_map;	
    }
    
    gpb_data = ioremap(GPBDAT, 4);
    if(!gpb_data)
    {
        printk(KERN_WARNING"led_data ioremap fail\n");
        goto err_map;	
    }
        
    writel(0x15400, gpb_con);
    
    return 0;
     
    err_map :
    	   return -EFAULT;        	
}

/*key_hardware*/
void key_hardware_init(void)
{
    unsigned int data;
    /*ӳ��Ĵ�����ַ*/
    gpg_con = ioremap(GPGCON, 4);
    if(!gpg_con)
    {
        printk(KERN_WARNING"map gpg_con fail!\n");             	
    }
    
    gpg_data = ioremap(GPGDAT, 4);
    if(!gpg_data)
    {
        printk(KERN_WARNING"map gpg_data fail!\n");             	
    }
    
    /*����key_port����*/
    data = readl(gpg_con);
    data &= ~0xc0c3;
    data |= 0x8082;
    writel(data, gpg_con);    	
}

/*����/��ʼ�� key_fops*/
struct file_operations key_fops=
{
    .open = key_open,
    .read = key_read,    	
};

/*����/��ʼ�������豸������*/
struct miscdevice key_miscdev=
{
    .minor = 200,
    .name = "my_key",
    .fops = &key_fops,	
};


static int __init button_init(void)
{    
    int ret;
    /*ע������豸*/
    ret = misc_register(&key_miscdev);
    if(ret)
    {
        printk(KERN_WARNING"add misc fial!\n");
        goto err_add;    	
    }
    
    /*��ʼ��ledӲ��*/
    led_hardware_init();
    /*��ʼ��keyӲ��*/
    key_hardware_init();
    
    /*ע���ж�*/
    ret = request_irq(IRQ_EINT1, key1_interrpt, IRQF_TRIGGER_FALLING, "my_key", 0);
    if(ret)
    {
        printk(KERN_WARNING"add irq8 fial!\n");
        goto err_iqr;
    }
    
    ret = request_irq(IRQ_EINT15, key1_interrpt, IRQF_TRIGGER_FALLING, "my_key", 0);
    if(ret)
    {
        printk(KERN_WARNING"add irq15 fial!\n");
        goto err_iqr;
    }
    
    ret = request_irq(IRQ_EINT11, key2_interrpt, IRQF_TRIGGER_FALLING, "my_key", 0);
    if(ret)
    {
        printk(KERN_WARNING"add irq11 fial!\n");
        goto err_iqr;
    }
    
    /*��������1*/
    work1 = kmalloc(sizeof(struct work_struct), GFP_KERNEL);
    if(!work1)
    {
        printk(KERN_WARNING"get work1 fial!\n");
        ret = -ENOMEM;
        goto err_kmalloc;    	
    }
    INIT_WORK(work1, work1_func); 
    
    /*��������2*/
    work2 = kmalloc(sizeof(struct work_struct), GFP_KERNEL);
    if(!work2)
    {
        printk(KERN_WARNING"get work1 fial!\n");
        ret = -ENOMEM;
        goto err_kmalloc;    	
    }
    INIT_WORK(work2, work2_func); 
    
    /*��ʼ����ʱ��*/
    init_timer(&key_timer);
    key_timer.expires =  jiffies + HZ/10; //100ms
    key_timer.function = key_timer_func; 
    
    /*ע�ᶨʱ��*/
    add_timer(&key_timer);
      
    /*��ʼ���ȴ�����*/
    init_waitqueue_head(&read_wait);
    
    return 0;
    
    err_kmalloc:
         kfree(work1);
         kfree(work2);
    err_iqr:
         free_irq(IRQ_EINT8, 0);
         free_irq(IRQ_EINT11, 0);
    err_add:
        misc_deregister(&key_miscdev);  
        return ret;
}


static void __exit button_exit(void)
{   
     /*ע����ʱ��*/
     del_timer(&key_timer);
    
    /*�ͷŹ���*/
     kfree(work1);
     kfree(work2);
     
    /*ע���ж�*/
     free_irq(IRQ_EINT8, 0);
     free_irq(IRQ_EINT15, 0);
     free_irq(IRQ_EINT11, 0);
     
    /*ע���豸*/ 
     misc_deregister(&key_miscdev);
       	
}


module_init(button_init);
module_exit(button_exit);

MODULE_LICENSE("GPL");
