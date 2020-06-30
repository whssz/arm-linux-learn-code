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
        /*中断实现上半部,硬件操作*/
        writel(0x61f, gpb_data);
        key_num = 1;
        
        /*中断实现下半部,提交工作*/
        schedule_work(work1);        	
    }
    
    if(0 == (data&0x80))
    {
        /*中断实现上半部,硬件操作*/
        writel(0x61f, gpb_data);
        key_num = 5;
        
        /*中断实现下半部,提交工作*/
        schedule_work(work1);    	
    }
    
    if(0 == (data&0x08))
    {
        /*中断实现上半部,硬件操作*/
        writel(0x3ff, gpb_data);
        key_num = 2;
        
        /*中断实现下半部,提交工作*/
        schedule_work(work2);   	
    }
    
    /*唤醒等待进程*/
    wake_up(&read_wait);
    
    return 0;	
}
/*work1_func*/
static int work1_func(void)
{    
    /*下半部*/
     printk(KERN_WARNING"led on!\n");  
     return 0;  	
}


/*work2_func*/
static int work2_func(void)
{    
    /*下半部*/
     printk(KERN_WARNING"led off!\n");
     return 0;   	
}


/*irq_handle*/
static irqreturn_t key1_interrpt(int irq, void *handle)
{
    /*检验是否产生中断*/
    
    /*清除中断,cpu自动清除*/
    
    /*按键消抖,定时器计时*/
    mod_timer(&key_timer, jiffies + HZ/10);
            
    return IRQ_HANDLED;    	
}

static irqreturn_t key2_interrpt(int irq, void *handle)
{
    /*检验是否产生中断*/
    
    /*清除中断,cpu自动清除*/
    
    /*按键消抖,定时器计时*/
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
    
    /*判断是否进入等待队列, 以key_num为判读.*/
    wait_event(read_wait, key_num);
    
    /*读取key_num*/
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
    /*映射寄存器地址*/
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
    
    /*配置key_port功能*/
    data = readl(gpg_con);
    data &= ~0xc0c3;
    data |= 0x8082;
    writel(data, gpg_con);    	
}

/*定义/初始化 key_fops*/
struct file_operations key_fops=
{
    .open = key_open,
    .read = key_read,    	
};

/*分配/初始化混杂设备描述符*/
struct miscdevice key_miscdev=
{
    .minor = 200,
    .name = "my_key",
    .fops = &key_fops,	
};


static int __init button_init(void)
{    
    int ret;
    /*注册混杂设备*/
    ret = misc_register(&key_miscdev);
    if(ret)
    {
        printk(KERN_WARNING"add misc fial!\n");
        goto err_add;    	
    }
    
    /*初始化led硬件*/
    led_hardware_init();
    /*初始化key硬件*/
    key_hardware_init();
    
    /*注册中断*/
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
    
    /*创建工作1*/
    work1 = kmalloc(sizeof(struct work_struct), GFP_KERNEL);
    if(!work1)
    {
        printk(KERN_WARNING"get work1 fial!\n");
        ret = -ENOMEM;
        goto err_kmalloc;    	
    }
    INIT_WORK(work1, work1_func); 
    
    /*创建工作2*/
    work2 = kmalloc(sizeof(struct work_struct), GFP_KERNEL);
    if(!work2)
    {
        printk(KERN_WARNING"get work1 fial!\n");
        ret = -ENOMEM;
        goto err_kmalloc;    	
    }
    INIT_WORK(work2, work2_func); 
    
    /*初始化定时器*/
    init_timer(&key_timer);
    key_timer.expires =  jiffies + HZ/10; //100ms
    key_timer.function = key_timer_func; 
    
    /*注册定时器*/
    add_timer(&key_timer);
      
    /*初始化等待队列*/
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
     /*注销定时器*/
     del_timer(&key_timer);
    
    /*释放工作*/
     kfree(work1);
     kfree(work2);
     
    /*注销中断*/
     free_irq(IRQ_EINT8, 0);
     free_irq(IRQ_EINT15, 0);
     free_irq(IRQ_EINT11, 0);
     
    /*注销设备*/ 
     misc_deregister(&key_miscdev);
       	
}


module_init(button_init);
module_exit(button_exit);

MODULE_LICENSE("GPL");
