#include <linux/module.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/platform_device.h>
#include <linux/kthread.h>
#include <linux/delay.h>

struct resource *mem_resource;
struct resource *mem_resource2;
unsigned int *register_base;
unsigned int *register_base2;

/*����led*/
void led_control()
{
    unsigned int val=0; 
    val = readl(register_base2);
   
    val = 0x15400;
    writel(val, register_base2);
    
    val = readl(register_base2+1);
    val = 0x3ff;
    writel(val , register_base2+1);	
}
/*���Ժ���*/
void port_test(void)
{
    unsigned int val=0;
    val = readl(register_base);
    //printk("%x\n", val);
    val &= ~(0x3<<2);
    val |= 0x10;
    writel(val, register_base);
       
    /*2.��ʼ״̬Ϊ�ߵ�ƽ*/
    val = readl(register_base+1);
    val |= (0x1<<1);
    writel(val, register_base+1);
    
    mdelay(20);
    
    val = readl(register_base+1);
    val &= ~(0x1<<1);
    writew(val, register_base+1);
    
    mdelay(20);
}
/*��ȡ���ŵĵ�ƽ*/
unsigned char port_data(void)
{
    unsigned char val =0;
    val =  readw(register_base+1);
    val &= (0x1<<1);
    return val;
   
}

/*�������Ϳ�ʼָ��*/
void DHT11_Rst(void)	   
{      
    unsigned int val1=0;
    //G1����Ϊ���           
    val1 = readl(register_base);
    val1 &= ~(0x3<<2);
    val1 = 0x1<<2;
    writel(val1, register_base); 
    
    //����G1,����18ms 	  
    val1 = readw(register_base+1);
    val1 &= ~(0x1<<1);
    writew(val1, register_base+1);  
    mdelay(20);    	  
    
    //����,�ȴ��ӻ���Ӧ,����20~40us
    val1 = readw(register_base+1);
    val1 |= 0x1<<1;
    writew(val1, register_base+1);        
    udelay(30);         	 	
}

/*���ӻ���Ӧ*/
unsigned char DHT11_Check(void) 	   
{   
    unsigned int val1=0;
    unsigned char retry=0;
    
    /*��������Ϊ����ģʽ*/
    val1 = readl(register_base);
    val1 &= ~(0x3<<2);
    writel(val1, register_base); 
    
    /*DHT11������40~80us*/ 
    while((port_data()) && (retry<100))
    {
	retry++;
	udelay(1);
    };	    
    if(retry>=100)
    { 
        return 1;
    }
    else retry=0;
    
    /*DHT11���ͺ���ٴ�����40~80us*/
    while((!port_data()) && (retry<100))
    {
	retry++;
	udelay(1);
    };
    if(retry>=100)
      { 
        return 1;
       } 	    
    return 0;
}

/*���ų�ʼ������*/
unsigned char dht_port_init(void)
{
    int ret=0;
    unsigned int val=0;
     
    /*1.��������Ϊ���*/  
    val = readl(register_base);
    val &= ~(0x3<<2);
    val |= 0x1<<2;
    writel(val, register_base);
       
    /*2.��ʼ״̬Ϊ�ߵ�ƽ*/
    val = readw(register_base+1);
    val |= 0x1<<1;
    writew(val, register_base+1);
    
    /*3.�������迪��*/
    val = readw(register_base+2);
    val &= ~(0x1<<1);
    writew(val, register_base+2);
    
    /*4.��λһ��,�ȴ��ӻ���Ӧ*/
    DHT11_Rst();
    
    /*5.���ӻ��Ƿ�Ӧ��*/
    ret = DHT11_Check();
    
    return ret;
}
/*��ȡһbit*/
unsigned char DHT11_Read_Bit(void) 			 
{
 	unsigned char retry=0;
	while((port_data()) && (retry<100))//�ȴ���Ϊ�͵�ƽ
	{
		retry++;
		udelay(1);
	}
	retry=0;
	while((!port_data()) && (retry<100))//�ȴ���ߵ�ƽ
	{
		retry++;
		udelay(1);
	}
	
	udelay(40);//�ȴ�40us
	
	if(port_data())
	    return 1;
	else 
	    return 0;		   
}
/*��ȡһ���ֽ�*/
unsigned char DHT11_Read_Byte(void)    
{        
    unsigned char i,dat;
    dat=0;
    for (i=0;i<8;i++) 
    {
   	dat<<=1; 
	dat|=DHT11_Read_Bit();
    }						    
    return dat;
}
/*��ȡһ֡����*/
unsigned char DHT11_Read_Data(unsigned char *envir)    
{       
 	unsigned char buf[5];
	unsigned char i;
	DHT11_Rst();
	
	if(DHT11_Check()==0)
	{
		for(i=0;i<5;i++)//��ȡ40λ����
		{
			buf[i]=DHT11_Read_Byte();
		}
		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
		{
			*envir=buf[0];
			*(envir+1)=buf[2];
		}
	}
	else 
	    return 1;
	
	return 0;	    
}


/*dhtmsc_open*/
int dhtmsc_open(struct inode *node, struct file *filp)
{  
  
   /*1.���ų�ʼ��*/
   while(dht_port_init());
    
    return 0;	
}

/*dhtmsc_read*/
ssize_t dhtmsc_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
    int ret=0;
    int ret2=0;
    unsigned char envir[2]={0};
    
    /*1.��ȡ����*/
    ret = DHT11_Read_Data(envir);
    
    /*2.���ݴ����û��ռ�*/
    if(1 == ret)
        printk(KERN_WARNING"read the envir fail!\n\r");        
    else
    {
       ret2 = copy_to_user(buf, envir, count); 
       if(ret2 < 0)
           return -EFAULT;
    }     
    
    return 0;	
}

static int dhtmsc_release(struct inode *node, struct file *filp)
{
     kfree(mem_resource);
     return 0;   	
}

/*��������豸�ļ�������*/
const struct file_operations dhtmsc_ops=
{
    .open = dhtmsc_open,
    .read = dhtmsc_read,
    .release = dhtmsc_release,  
};

/*��������ַ��豸*/
struct miscdevice dht_mscdrv=
{
    .minor = 0,
    .name = "dht_mscdrv",
    .fops = &dhtmsc_ops,	
};

/*dht_probe*/
static int dht_probe(struct platform_device *pdev)
{  
    int ret = 0;
    unsigned char size; 
    /*��ȡ��Դ*/
    mem_resource =  platform_get_resource(pdev, IORESOURCE_MEM, 0);
    if(!mem_resource)
    {
        printk(KERN_WARNING"get the resource fail!\n");
        return -ENODEV;        
    }
    
    /*ת����ַ*/
    size = (mem_resource->end) - (mem_resource->start);
    register_base = ioremap(mem_resource->start, size);
    if(!register_base)
    {
         printk(KERN_WARNING"map the register_base fail!\n");
         return -EFAULT;          	
    }
    
    /*mem_resource2= platform_get_resource(pdev, IORESOURCE_MEM, 1);
    size = (mem_resource2->end) - (mem_resource2->start);
    register_base2 = ioremap(mem_resource2->start, size);
    
    led_control();
     while(1)
         port_test();
    */
    /*ע������豸*/
    ret = misc_register(&dht_mscdrv);
    if(ret)
    {
        goto err_misc;	
    }
  
    return 0;
    
    err_misc: 
        misc_deregister(&dht_mscdrv);
        return ret;   
 }

/*my_remove*/
static int dht_remove(struct platform_device *pdev)
{
    misc_deregister(&dht_mscdrv);
    return 0;	
}

/*����ƽ̨����������*/
struct platform_driver dht_drv=
{
    .probe = dht_probe, 
    .remove =  dht_remove,
    .driver = {
    	          .owner = THIS_MODULE, 
    		  .name = "dht_dev",		  	
              },
};

static int __init dht_init(void)
{	 
    int ret;
    
    ret = platform_driver_register(&dht_drv);
    if(ret)
    {
        printk(KERN_WARNING"add dht11 driver fail!\n");
        goto err_add;
    }  
    	
    return 0;
    
    err_add:
    	platform_driver_unregister(&dht_drv); 
        return ret;  	
}

static void __exit dht_exit(void)
{
   ;	
}

module_init(dht_init);
module_exit(dht_exit);

MODULE_LICENSE("GPL");