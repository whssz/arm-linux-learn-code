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

/*控制led*/
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
/*测试函数*/
void port_test(void)
{
    unsigned int val=0;
    val = readl(register_base);
    //printk("%x\n", val);
    val &= ~(0x3<<2);
    val |= 0x10;
    writel(val, register_base);
       
    /*2.初始状态为高电平*/
    val = readl(register_base+1);
    val |= (0x1<<1);
    writel(val, register_base+1);
    
    mdelay(20);
    
    val = readl(register_base+1);
    val &= ~(0x1<<1);
    writew(val, register_base+1);
    
    mdelay(20);
}
/*获取引脚的电平*/
unsigned char port_data(void)
{
    unsigned char val =0;
    val =  readw(register_base+1);
    val &= (0x1<<1);
    return val;
   
}

/*主机发送开始指令*/
void DHT11_Rst(void)	   
{      
    unsigned int val1=0;
    //G1设置为输出           
    val1 = readl(register_base);
    val1 &= ~(0x3<<2);
    val1 = 0x1<<2;
    writel(val1, register_base); 
    
    //拉低G1,至少18ms 	  
    val1 = readw(register_base+1);
    val1 &= ~(0x1<<1);
    writew(val1, register_base+1);  
    mdelay(20);    	  
    
    //拉高,等待从机回应,拉高20~40us
    val1 = readw(register_base+1);
    val1 |= 0x1<<1;
    writew(val1, register_base+1);        
    udelay(30);         	 	
}

/*检查从机回应*/
unsigned char DHT11_Check(void) 	   
{   
    unsigned int val1=0;
    unsigned char retry=0;
    
    /*设置引脚为输入模式*/
    val1 = readl(register_base);
    val1 &= ~(0x3<<2);
    writel(val1, register_base); 
    
    /*DHT11会拉低40~80us*/ 
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
    
    /*DHT11拉低后会再次拉高40~80us*/
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

/*引脚初始化函数*/
unsigned char dht_port_init(void)
{
    int ret=0;
    unsigned int val=0;
     
    /*1.配置引脚为输出*/  
    val = readl(register_base);
    val &= ~(0x3<<2);
    val |= 0x1<<2;
    writel(val, register_base);
       
    /*2.初始状态为高电平*/
    val = readw(register_base+1);
    val |= 0x1<<1;
    writew(val, register_base+1);
    
    /*3.上拉电阻开启*/
    val = readw(register_base+2);
    val &= ~(0x1<<1);
    writew(val, register_base+2);
    
    /*4.复位一次,等待从机响应*/
    DHT11_Rst();
    
    /*5.检测从机是否应答*/
    ret = DHT11_Check();
    
    return ret;
}
/*读取一bit*/
unsigned char DHT11_Read_Bit(void) 			 
{
 	unsigned char retry=0;
	while((port_data()) && (retry<100))//等待变为低电平
	{
		retry++;
		udelay(1);
	}
	retry=0;
	while((!port_data()) && (retry<100))//等待变高电平
	{
		retry++;
		udelay(1);
	}
	
	udelay(40);//等待40us
	
	if(port_data())
	    return 1;
	else 
	    return 0;		   
}
/*读取一个字节*/
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
/*读取一帧数据*/
unsigned char DHT11_Read_Data(unsigned char *envir)    
{       
 	unsigned char buf[5];
	unsigned char i;
	DHT11_Rst();
	
	if(DHT11_Check()==0)
	{
		for(i=0;i<5;i++)//读取40位数据
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
  
   /*1.引脚初始化*/
   while(dht_port_init());
    
    return 0;	
}

/*dhtmsc_read*/
ssize_t dhtmsc_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
    int ret=0;
    int ret2=0;
    unsigned char envir[2]={0};
    
    /*1.读取数据*/
    ret = DHT11_Read_Data(envir);
    
    /*2.数据传回用户空间*/
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

/*定义混在设备文件描述符*/
const struct file_operations dhtmsc_ops=
{
    .open = dhtmsc_open,
    .read = dhtmsc_read,
    .release = dhtmsc_release,  
};

/*定义混杂字符设备*/
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
    /*获取资源*/
    mem_resource =  platform_get_resource(pdev, IORESOURCE_MEM, 0);
    if(!mem_resource)
    {
        printk(KERN_WARNING"get the resource fail!\n");
        return -ENODEV;        
    }
    
    /*转化地址*/
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
    /*注册混杂设备*/
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

/*分配平台驱动描述符*/
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