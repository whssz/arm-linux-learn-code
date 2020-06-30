#include <linux/module.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/platform_device.h>
#include <linux/kthread.h>



#define GPGCON 0x56000060
#define GPGDAT 0x56000064

struct work_struct *work1;

struct timer_list buttons_timer;

unsigned int *gpio_data;

unsigned int key_num = 0;

wait_queue_head_t  key_q;

//static struct work_struct *work2;

void work1_func(struct work_struct *work)
{  
    //启动定时器,定时100ms
    mod_timer(&buttons_timer, jiffies + (HZ/10));
}


void buttons_timer_function(unsigned long data)
{  
   unsigned int key_val;
  
   key_val = readw(gpio_data)&0x1; 
   // printk("yes\n");
   if(0 == key_val)
      // printk("key1 was down!\n");
      key_num = 1;
     
   
   key_val = readw(gpio_data)&0x8; 
   // printk("yes\n");
   if(0 == key_val)
       //printk("key2 was down!\n"); 
        key_num = 2;
  
    wake_up_interruptible(&key_q); //产生了中断, 唤醒进程
       	
}


irqreturn_t key_int(int irq, void *dev_id)
{
    /*判断是否产生中断*/
    
    /*清除中断标志位*/
    
    /*打印按键号*/
   // printk("the interrpt is occur!\n");
   
   // schedule_work(work2);
   /*启动定时器*/
    schedule_work(work1);
    
    return IRQ_HANDLED;
}



void key_hw_init()
{    
    unsigned int *gpio_config;
    unsigned short data;
    /*IO口模式设置*/
    gpio_config = ioremap(GPGCON, 4);
    data = readw(gpio_config); //读出寄存器里面的值
    data &= ~0xc3;   //清0 最低两位
    data |= 0x82;   //设置为10
    
    writew(data, gpio_config);	
    
    gpio_data = ioremap(GPGDAT, 4); //数据寄存器的地址转换
}


int key_open(struct inode *node, struct file *filp)
{
    return 0;	
}

ssize_t key_read(struct file *filp, char __user *buf, size_t size, loff_t *pos)
{   
    /*判断是否需要入等待序列*/
     wait_event_interruptible(key_q,key_num);
    
    printk("don't wait!\n");
    copy_to_user(buf, &key_num, 4);
    
    key_num = 0;
    
   // printk("the key_num is %d\n", key_num);
    return 4;
}


struct file_operations key_fops =
{
    .open = key_open,	
    .read = key_read,
};

struct miscdevice key_miscdev=
{
    .minor = 200,
    .name = "key",
    .fops = &key_fops,	
};

static int button_init()
{
    misc_register(&key_miscdev);  //注册miscdevice
    
    request_irq(IRQ_EINT8, key_int, IRQF_TRIGGER_FALLING, "key", 0);  //注册中断
    request_irq(IRQ_EINT11, key_int, IRQF_TRIGGER_FALLING, "key", 0);  //注册中断
      
    key_hw_init(); //按键硬件初始化
    
    //为工作描述分配一段内存
    work1 = kmalloc(sizeof(struct work_struct), GFP_KERNEL); //GFP_KERNEL是内核内存分配时最常用的，无内存可用时可引起休眠
    INIT_WORK(work1, work1_func);
    /*挂载工作*/
    //queue_work(work_queue, work1);
   
    /*创建工作*/
    //为工作描述分配一段内存
    //work2 = kmalloc(sizeof(struct work_struct), GFP_KERNEL);
    //INIT_WORK(work2, work2_func);
    /*挂载工作*/
    // queue_work(work_queue, work2);
    
    /*初始化定时器,并定义超时函数*/
    init_timer(&buttons_timer);
    buttons_timer.function = buttons_timer_function;
    
    /*注册定时器*/
    add_timer(&buttons_timer);
    
    /*定义等待序列*/
    
    /*初始化等待队列*/
     init_waitqueue_head(&key_q); 

    return 0;	
}






static void button_exit()
{
   misc_deregister(&key_miscdev) ; //注销混杂设备
   
   free_irq(IRQ_EINT8, 0);	//注销中
   free_irq(IRQ_EINT11, 0);
}



MODULE_LICENSE("GPL");  //支持GPL开源协议
module_init(button_init);
module_exit(button_exit);
