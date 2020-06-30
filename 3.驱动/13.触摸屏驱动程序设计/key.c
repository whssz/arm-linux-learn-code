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
    //������ʱ��,��ʱ100ms
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
   
   input_sync(button_dev); //�ϱ����       	
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




static int button_init()
{
   /*�����������豸*/
    button_dev = input_allocate_device();
    
   /*����������������ϱ����¼�*/
    set_bit(EV_KEY, button_dev->evbit);
    
   /*����ǰ���,��Ҫ�����ϱ��İ������*/
    set_bit(KEY_1, button_dev->keybit);
    set_bit(KEY_2, button_dev->keybit);
    
   /*ע���������豸*/
    input_register_device(button_dev);
    
    request_irq(IRQ_EINT8, key_int, IRQF_TRIGGER_FALLING, "key", 0);  //ע���ж�
    request_irq(IRQ_EINT11, key_int, IRQF_TRIGGER_FALLING, "key", 0);  //ע���ж�
      
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

    return 0;	
}






static void button_exit()
{     
   /*ע���������豸*/
   input_unregister_device(button_dev);
   free_irq(IRQ_EINT8, 0);	//ע����
   free_irq(IRQ_EINT11, 0);
}



MODULE_LICENSE("GPL");  //֧��GPL��ԴЭ��
module_init(button_init);
module_exit(button_exit);
