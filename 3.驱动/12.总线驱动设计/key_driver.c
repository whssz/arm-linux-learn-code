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

unsigned int *gpio_data;

unsigned int key_num = 0;

wait_queue_head_t  key_q;

struct resource *mem_resource;
struct resource *irq_resource;

unsigned int *key_base;


/*工作1函数,启动定时器*/
void work1_func(struct work_struct *work)
{  
    //启动定时器,定时100ms
    mod_timer(&buttons_timer, jiffies + (HZ/10));
}

/*定时器计时完成,执行函数*/
void buttons_timer_function(unsigned long data)
{  
   unsigned int key_val;
  
   key_val = readw(key_base+1)&0x1;   //控制寄存器+1就是数据寄存器 
   if(0 == key_val)
      key_num = 1;
     
   
   key_val = readw(key_base+1)&0x8; 
   if(0 == key_val)    
        key_num = 2;
  
    wake_up_interruptible(&key_q); //产生了中断, 唤醒进程     	
}

/*按键中断函数*/
irqreturn_t key_int(int irq, void *dev_id)
{
    /*判断是否产生中断*/
    
    /*清除中断标志位*/
 
   
    
    //注册工作1
    schedule_work(work1);
    
    return IRQ_HANDLED;
}


/*按键硬件初始化*/
void key_hw_init()
{    
   
    unsigned short data;
    
    data = readw(key_base); //读出寄存器里面的值
    data &= ~0xc3;   //清0 最低两位
    data |= 0x82;   //设置为10
    
    writew(data, key_base);	
    
}

/*打开按键函数,功能函数*/
int key_open(struct inode *node, struct file *filp)
{
    return 0;	
}
/*读函数,功能函数*/
ssize_t key_read(struct file *filp, char __user *buf, size_t size, loff_t *pos)
{   
    /*判断是否需要入等待序列*/
    wait_event_interruptible(key_q,key_num);
    
    printk("don't wait!\n");
    copy_to_user(buf, &key_num, 4);
    
    key_num = 0;
     
    return 4;
}

/*操作函数集*/
struct file_operations key_fops =
{
    .open = key_open,	
    .read = key_read,
};
/*混杂设备描述结构*/
struct miscdevice key_miscdev=
{
    .minor = 200,
    .name = "key",
    .fops = &key_fops,	
};

/*平台总线上匹配设备,所要执行的函数*/
static int key_probe(struct platform_device *pdev)
{  
    int ret ,size;
    ret = misc_register(&key_miscdev);  //注册miscdevice
    
    irq_resource =  platform_get_resource(pdev, IORESOURCE_IRQ, 0); //从总线上得到匹配设备的中断资源
    
    request_irq(irq_resource->start, key_int, IRQF_TRIGGER_FALLING, "key", (void *)1);  //注册中断
    request_irq(irq_resource->end, key_int, IRQF_TRIGGER_FALLING, "key", (void *)2);  //注册中断
    
    mem_resource = platform_get_resource(pdev, IORESOURCE_MEM, 0); //从总线上的到匹配设备的内存资源
    size = (mem_resource->end) - (mem_resource->start) ; //有待考虑
    key_base = ioremap(mem_resource->start, size); //物理地址 到 虚拟地址转换, size个字节
    
    key_hw_init(); //按键硬件初始化
    
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
    
    return  ret;
}
/*注销驱动所要执行的函数*/
static int key_remove(struct platform_device *pdev)
{
    int ret = misc_deregister(&key_miscdev) ; //注销混杂设备  
    free_irq(IRQ_EINT8, 0);	//注销中断
    free_irq(IRQ_EINT11, 0); 
    return ret;
}

/*平台设备驱动描述结构*/
struct platform_driver key_driver=
{
    .probe = key_probe,
    .remove = __devexit_p(key_remove),
    .driver = {
		.name = "key_bus",
	      },
};

/*驱动进入函数*/
static int button_init()
{ 
    return  platform_driver_register(&key_driver); //挂载按键驱动到平台总线(系统默认总线)   	
}


/*驱动退出函数*/
static void button_exit()
{  
   platform_driver_unregister(&key_driver); //注销按键驱动
   
}



MODULE_LICENSE("GPL");  //支持GPL合法协议

module_init(button_init);
module_exit(button_exit);
