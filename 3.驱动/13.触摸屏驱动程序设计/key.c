#include <linux/module.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/platform_device.h>
#include <linux/kthread.h>
#include <linux/input.h>



#define GPGCON 0x56000060
#define GPGDAT 0x56000064

struct work_struct *work1;

struct timer_list buttons_timer;

unsigned int *gpio_data;

unsigned int key_num = 0;

wait_queue_head_t  key_q;

struct input_dev *button_dev;



void work1_func(struct work_struct *work)
{  
    //启动定时器,定时100ms
    mod_timer(&buttons_timer, jiffies + (HZ/10));
}


void buttons_timer_function(unsigned long data)
{  
   unsigned int key_val;
  
   key_val = readw(gpio_data)&0x1; 
  
   if(0 == key_val) 
   {   
      key_num = 1; 
      input_report_key(button_dev, KEY_1, 1);
   }  
   
   key_val = readw(gpio_data)&0x8; 
  
   if(0 == key_val)
   {
     key_num = 2;
     input_report_key(button_dev, KEY_2, 1);
   }
   
   input_sync(button_dev); //上报完毕       	
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




static int button_init()
{
   /*分配输入型设备*/
    button_dev = input_allocate_device();
    
   /*申明驱动会向核心上报的事件*/
    set_bit(EV_KEY, button_dev->evbit);
    
   /*如果是按键,需要申明上报的按键标号*/
    set_bit(KEY_1, button_dev->keybit);
    set_bit(KEY_2, button_dev->keybit);
    
   /*注册输入型设备*/
    input_register_device(button_dev);
    
    request_irq(IRQ_EINT8, key_int, IRQF_TRIGGER_FALLING, "key", 0);  //注册中断
    request_irq(IRQ_EINT11, key_int, IRQF_TRIGGER_FALLING, "key", 0);  //注册中断
      
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

    return 0;	
}






static void button_exit()
{     
   /*注销输入型设备*/
   input_unregister_device(button_dev);
   free_irq(IRQ_EINT8, 0);	//注销中
   free_irq(IRQ_EINT11, 0);
}



MODULE_LICENSE("GPL");  //支持GPL开源协议
module_init(button_init);
module_exit(button_exit);
