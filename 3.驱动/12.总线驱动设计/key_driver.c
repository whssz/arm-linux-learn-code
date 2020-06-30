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


/*����1����,������ʱ��*/
void work1_func(struct work_struct *work)
{  
    //������ʱ��,��ʱ100ms
    mod_timer(&buttons_timer, jiffies + (HZ/10));
}

/*��ʱ����ʱ���,ִ�к���*/
void buttons_timer_function(unsigned long data)
{  
   unsigned int key_val;
  
   key_val = readw(key_base+1)&0x1;   //���ƼĴ���+1�������ݼĴ��� 
   if(0 == key_val)
      key_num = 1;
     
   
   key_val = readw(key_base+1)&0x8; 
   if(0 == key_val)    
        key_num = 2;
  
    wake_up_interruptible(&key_q); //�������ж�, ���ѽ���     	
}

/*�����жϺ���*/
irqreturn_t key_int(int irq, void *dev_id)
{
    /*�ж��Ƿ�����ж�*/
    
    /*����жϱ�־λ*/
 
   
    
    //ע�Ṥ��1
    schedule_work(work1);
    
    return IRQ_HANDLED;
}


/*����Ӳ����ʼ��*/
void key_hw_init()
{    
   
    unsigned short data;
    
    data = readw(key_base); //�����Ĵ��������ֵ
    data &= ~0xc3;   //��0 �����λ
    data |= 0x82;   //����Ϊ10
    
    writew(data, key_base);	
    
}

/*�򿪰�������,���ܺ���*/
int key_open(struct inode *node, struct file *filp)
{
    return 0;	
}
/*������,���ܺ���*/
ssize_t key_read(struct file *filp, char __user *buf, size_t size, loff_t *pos)
{   
    /*�ж��Ƿ���Ҫ��ȴ�����*/
    wait_event_interruptible(key_q,key_num);
    
    printk("don't wait!\n");
    copy_to_user(buf, &key_num, 4);
    
    key_num = 0;
     
    return 4;
}

/*����������*/
struct file_operations key_fops =
{
    .open = key_open,	
    .read = key_read,
};
/*�����豸�����ṹ*/
struct miscdevice key_miscdev=
{
    .minor = 200,
    .name = "key",
    .fops = &key_fops,	
};

/*ƽ̨������ƥ���豸,��Ҫִ�еĺ���*/
static int key_probe(struct platform_device *pdev)
{  
    int ret ,size;
    ret = misc_register(&key_miscdev);  //ע��miscdevice
    
    irq_resource =  platform_get_resource(pdev, IORESOURCE_IRQ, 0); //�������ϵõ�ƥ���豸���ж���Դ
    
    request_irq(irq_resource->start, key_int, IRQF_TRIGGER_FALLING, "key", (void *)1);  //ע���ж�
    request_irq(irq_resource->end, key_int, IRQF_TRIGGER_FALLING, "key", (void *)2);  //ע���ж�
    
    mem_resource = platform_get_resource(pdev, IORESOURCE_MEM, 0); //�������ϵĵ�ƥ���豸���ڴ���Դ
    size = (mem_resource->end) - (mem_resource->start) ; //�д�����
    key_base = ioremap(mem_resource->start, size); //�����ַ �� �����ַת��, size���ֽ�
    
    key_hw_init(); //����Ӳ����ʼ��
    
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
    
    return  ret;
}
/*ע��������Ҫִ�еĺ���*/
static int key_remove(struct platform_device *pdev)
{
    int ret = misc_deregister(&key_miscdev) ; //ע�������豸  
    free_irq(IRQ_EINT8, 0);	//ע���ж�
    free_irq(IRQ_EINT11, 0); 
    return ret;
}

/*ƽ̨�豸���������ṹ*/
struct platform_driver key_driver=
{
    .probe = key_probe,
    .remove = __devexit_p(key_remove),
    .driver = {
		.name = "key_bus",
	      },
};

/*�������뺯��*/
static int button_init()
{ 
    return  platform_driver_register(&key_driver); //���ذ���������ƽ̨����(ϵͳĬ������)   	
}


/*�����˳�����*/
static void button_exit()
{  
   platform_driver_unregister(&key_driver); //ע����������
   
}



MODULE_LICENSE("GPL");  //֧��GPL�Ϸ�Э��

module_init(button_init);
module_exit(button_exit);
