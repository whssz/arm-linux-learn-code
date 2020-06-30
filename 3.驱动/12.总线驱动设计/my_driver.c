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
    /*�ж��Ƿ���Ҫ��ȴ�����*/
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
    /*�ж��Ƿ�����ж�*/
    
    /*����жϱ�־λ*/
     
   /*�ύ����*/
    schedule_work(work1);
    
    return IRQ_HANDLED;
}

void work1_func(struct work_struct *work)
{  
    //������ʱ��,��ʱ100ms
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
  
   wake_up_interruptible(&key_q); //�������ж�, ���ѽ���
       	
}



void key_hw_init()
{    
   
    unsigned short data;
    /*IO��ģʽ����*/
  
    data = readl(add_base); //�����Ĵ��������ֵ
    data &= ~0xc3;   //��0 �����λ
    data |= 0x82;   //����Ϊ10
    
    writel(data,add_base);	
    
}

/*probe����, ʵ���ҵ��豸�Ĳ���*/
static int key_probe(struct platform_device *pdev)
{   
    int ret=0;
    unsigned int size =0;
    misc_register(&key_miscdev);  //ע��miscdevice
    
    /*��ȡ�豸����Դ*/
    irqresource1 = platform_get_resource(pdev, IORESOURCE_IRQ, 0);
   // irqresource2 = platform_get_resource(pdev, IORESOURCE_IRQ, 1);
    //if(irqresource2 < 0)
  //  {
       // printk("get irq2 fail!\n");
       // ret = irqresource2;
       // return irqresource2;
   // }
    request_irq(irqresource1->start, key_int, IRQF_TRIGGER_FALLING, "my_key", 0);  //ע���ж�
    request_irq(irqresource1->end-1, key_int, IRQF_TRIGGER_FALLING, "my_key", 0);  //ע���ж�
    
    memresource = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    size = (memresource->end) - (memresource->start);
    add_base = ioremap(memresource->start, size);
    
     /*����Ӳ����ʼ��*/
    key_hw_init();
    
    
    //Ϊ������������һ���ڴ�
    work1 = kmalloc(sizeof(struct work_struct), GFP_KERNEL); //GFP_KERNEL���ں��ڴ����ʱ��õģ����ڴ����ʱ����������
    INIT_WORK(work1, work1_func);   
    
    /*��ʼ����ʱ��,�����峬ʱ����*/
    init_timer(&buttons_timer);    
    buttons_timer.function = buttons_timer_function;
    
    /*ע�ᶨʱ��*/
    add_timer(&buttons_timer);
      
    /*��ʼ���ȴ�����*/
    init_waitqueue_head(&key_q); 
   
    return 0;   	
}

/*remove����,ʵ���豸ɾ���Ĳ���*/
static int key_remove(struct platform_device *pdev)
{
   misc_deregister(&key_miscdev) ; //ע�������豸
   
   free_irq(irqresource1->start, 0);//ע���ж�
   free_irq(irqresource1->end-1, 0);
   
   return 0;	
}

/*����ƽ̨�豸����*/
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
     
     /*ע��ƽ̨����*/
     platform_driver_register(&key_driver);
     
    return 0;	
}


static void button_exit()
{
    /*ע��ƽ̨����*/
    platform_driver_unregister(&key_driver);
}



MODULE_LICENSE("GPL");  //֧��GPL��ԴЭ��

module_init(button_init);
module_exit(button_exit);
