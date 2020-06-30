#include <linux/module.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/platform_device.h>
#include <linux/kthread.h>


struct work_struct *work1;
struct timer_list buttons_timer;
unsigned int *add_base;
unsigned int key_num = 0;
wait_queue_head_t  key_q;

struct resource *irqresource1;
//struct resource *irqresource2;
struct resource *memresource;


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
    .name = "my_key",
    .fops = &key_fops,	
};


irqreturn_t key_int(int irq, void *dev_id)
{
    /*判断是否产生中断*/
    
    /*清除中断标志位*/
     
   /*提交工作*/
    schedule_work(work1);
    
    return IRQ_HANDLED;
}

void work1_func(struct work_struct *work)
{  
    //启动定时器,定时100ms
    mod_timer(&buttons_timer, jiffies + (HZ/10));
}


void buttons_timer_function(unsigned long data)
{  
   unsigned int key_val;
  
   key_val = readw(add_base+1)&0x1; 
  
   if(0 == key_val)
      key_num = 1;
     
   
   key_val = readw(add_base+1)&0x8; 
  
   if(0 == key_val)       
        key_num = 2;
  
   wake_up_interruptible(&key_q); //产生了中断, 唤醒进程
       	
}



void key_hw_init()
{    
   
    unsigned short data;
    /*IO口模式设置*/
  
    data = readl(add_base); //读出寄存器里面的值
    data &= ~0xc3;   //清0 最低两位
    data |= 0x82;   //设置为10
    
    writel(data,add_base);	
    
}

/*probe函数, 实现找到设备的操作*/
static int key_probe(struct platform_device *pdev)
{   
    int ret=0;
    unsigned int size =0;
    misc_register(&key_miscdev);  //注册miscdevice
    
    /*获取设备的资源*/
    irqresource1 = platform_get_resource(pdev, IORESOURCE_IRQ, 0);
   // irqresource2 = platform_get_resource(pdev, IORESOURCE_IRQ, 1);
    //if(irqresource2 < 0)
  //  {
       // printk("get irq2 fail!\n");
       // ret = irqresource2;
       // return irqresource2;
   // }
    request_irq(irqresource1->start, key_int, IRQF_TRIGGER_FALLING, "my_key", 0);  //注册中断
    request_irq(irqresource1->end-1, key_int, IRQF_TRIGGER_FALLING, "my_key", 0);  //注册中断
    
    memresource = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    size = (memresource->end) - (memresource->start);
    add_base = ioremap(memresource->start, size);
    
     /*按键硬件初始化*/
    key_hw_init();
    
    
    //为工作描述分配一段内存
    work1 = kmalloc(sizeof(struct work_struct), GFP_KERNEL); //GFP_KERNEL是内核内存分配时最常用的，无内存可用时可引起休眠
    INIT_WORK(work1, work1_func);   
    
    /*初始化定时器,并定义超时函数*/
    init_timer(&buttons_timer);    
    buttons_timer.function = buttons_timer_function;
    
    /*注册定时器*/
    add_timer(&buttons_timer);
      
    /*初始化等待队列*/
    init_waitqueue_head(&key_q); 
   
    return 0;   	
}

/*remove函数,实现设备删除的操作*/
static int key_remove(struct platform_device *pdev)
{
   misc_deregister(&key_miscdev) ; //注销混杂设备
   
   free_irq(irqresource1->start, 0);//注销中断
   free_irq(irqresource1->end-1, 0);
   
   return 0;	
}

/*定义平台设备驱动*/
struct platform_driver key_driver=
{
    .probe = key_probe,
    .remove = key_remove,
    .driver = {
    	          .owner = THIS_MODULE, 
    		  .name = "my_key",		  	
              },
};


static int button_init()
{
     
     /*注册平台驱动*/
     platform_driver_register(&key_driver);
     
    return 0;	
}


static void button_exit()
{
    /*注销平台驱动*/
    platform_driver_unregister(&key_driver);
}



MODULE_LICENSE("GPL");  //支持GPL开源协议

module_init(button_init);
module_exit(button_exit);
