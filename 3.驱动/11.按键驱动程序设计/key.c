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
    //������ʱ��,��ʱ100ms
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
  
    wake_up_interruptible(&key_q); //�������ж�, ���ѽ���
       	
}


irqreturn_t key_int(int irq, void *dev_id)
{
    /*�ж��Ƿ�����ж�*/
    
    /*����жϱ�־λ*/
    
    /*��ӡ������*/
   // printk("the interrpt is occur!\n");
   
   // schedule_work(work2);
   /*������ʱ��*/
    schedule_work(work1);
    
    return IRQ_HANDLED;
}



void key_hw_init()
{    
    unsigned int *gpio_config;
    unsigned short data;
    /*IO��ģʽ����*/
    gpio_config = ioremap(GPGCON, 4);
    data = readw(gpio_config); //�����Ĵ��������ֵ
    data &= ~0xc3;   //��0 �����λ
    data |= 0x82;   //����Ϊ10
    
    writew(data, gpio_config);	
    
    gpio_data = ioremap(GPGDAT, 4); //���ݼĴ����ĵ�ַת��
}


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
    misc_register(&key_miscdev);  //ע��miscdevice
    
    request_irq(IRQ_EINT8, key_int, IRQF_TRIGGER_FALLING, "key", 0);  //ע���ж�
    request_irq(IRQ_EINT11, key_int, IRQF_TRIGGER_FALLING, "key", 0);  //ע���ж�
      
    key_hw_init(); //����Ӳ����ʼ��
    
    //Ϊ������������һ���ڴ�
    work1 = kmalloc(sizeof(struct work_struct), GFP_KERNEL); //GFP_KERNEL���ں��ڴ����ʱ��õģ����ڴ����ʱ����������
    INIT_WORK(work1, work1_func);
    /*���ع���*/
    //queue_work(work_queue, work1);
   
    /*��������*/
    //Ϊ������������һ���ڴ�
    //work2 = kmalloc(sizeof(struct work_struct), GFP_KERNEL);
    //INIT_WORK(work2, work2_func);
    /*���ع���*/
    // queue_work(work_queue, work2);
    
    /*��ʼ����ʱ��,�����峬ʱ����*/
    init_timer(&buttons_timer);
    buttons_timer.function = buttons_timer_function;
    
    /*ע�ᶨʱ��*/
    add_timer(&buttons_timer);
    
    /*����ȴ�����*/
    
    /*��ʼ���ȴ�����*/
     init_waitqueue_head(&key_q); 

    return 0;	
}






static void button_exit()
{
   misc_deregister(&key_miscdev) ; //ע�������豸
   
   free_irq(IRQ_EINT8, 0);	//ע����
   free_irq(IRQ_EINT11, 0);
}



MODULE_LICENSE("GPL");  //֧��GPL��ԴЭ��
module_init(button_init);
module_exit(button_exit);
