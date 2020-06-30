#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/i2c.h>  
#include <linux/i2c-dev.h> 
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/smp_lock.h>
#include <asm/uaccess.h>  
#include <linux/delay.h> 
#include <linux/cdev.h>
#include <linux/init.h>

#define I2C_CDEV_MAJOR 113
#define DEVICE_NAME "gy50"

static struct i2c_client *gy50_i2c_client;  /*ȫ�ֱ���*/
struct class *gy50_class;

/*��������֧�ֵ��豸�б�*/
static struct i2c_device_id gy50_idtable[] = {
	
    {"l3g4200d", 0x69},	
    {}	
};
MODULE_DEVICE_TABLE(i2c, gy50_idtable);


/*i2c_driver �� probe ����*/
static int gy50_i2c_probe(struct i2c_client *client, const struct i2c_device_id *id)  
{ 
    struct device *dev; 
    
    printk(KERN_INFO"probe:name = %s,flag =%d,addr = %d,adapter = %d,driver = %s\n",client->name,  
         client->flags,client->addr,client->adapter->nr,client->driver->driver.name );	
         
    dev = device_create(gy50_class, &(client->dev), MKDEV(I2C_CDEV_MAJOR, 0), NULL, DEVICE_NAME); /*�����ļ��豸�ڵ�, MKDEV(I2C_CDEV_MAJOR, 0)���豸�źʹ��豸�źϳɵ��豸��*/
    if (IS_ERR(dev))  
    {  
        printk(KERN_WARNING"device create error\n"); 
        device_destroy(gy50_class, 0);
        return -1;  
    }
        
    gy50_i2c_client = client;  /*��ȡi2c_client ����Ϣ,����Ҫ��һ��*/
    
    return 0;	    
}

/*i2c_driver �� remove ����*/
static int  gy50_i2c_remove(struct i2c_client *client)  
{   
    printk(KERN_WARNING"remove\n");  
    return 0;  
} 

/*����i2c_driver�ṹ*/
static struct i2c_driver gy50_i2c_driver = {
	
    .driver = {
        .name = "i2c_gy50",
        .owner = THIS_MODULE,	
    },
   .probe = gy50_i2c_probe,
   .remove = __devexit_p(gy50_i2c_remove),  /*__devexit_p ����ֻ���ڰ�װ��������������Ż���Ч*/
   .id_table = gy50_idtable,  /*�豸��*/
};

/*�ַ��豸 open ����*/
static int gy50_open(struct inode *inode, struct file *fd)
{
    fd->private_data  = (void *)gy50_i2c_client; /*�ļ���˽�����ݱ��� �豸�ṹ*/
    
    return 0;	
}

/*�ַ��豸 release ����*/
static int gy50_close(struct inode *inode, struct file *fd)  
{  
    printk(KERN_WARNING"release\n");  
    
    fd->private_data = NULL;  /*����ļ�˽����Ϣ*/
      
    return 0;      
} 

/*�ַ��豸 read ����*/
/*�ļ���loff_t ��һ������,����һ��ָ��*/
static ssize_t gy50_read(struct file *fd, char __user *buf, size_t count, loff_t *f_pos)
{ 
    int ret;  
    unsigned char reg_addr =(unsigned char)(*f_pos);  /*��ȡ�Ĵ�����ַ*/
    struct i2c_client *client = (struct i2c_client*) fd->private_data;  /*��ȡclient����Ϣ*/
    struct i2c_msg *msgs; 
    msgs = kmalloc(2*sizeof(struct i2c_msg),GFP_KERNEL);
    if(!msgs)
    {
         printk(KERN_WARNING"get the malloce for msgs fail!\n");
         goto out;	
    }
    
   /*��ȡ�Ĵ�����ַ*/
        
    msgs[0].addr = client->addr;  
    msgs[0].flags = 0;  
    msgs[0].len = 1; 
    msgs[0].buf = kmalloc(1,GFP_KERNEL);
    if(!msgs[0].buf)
    {
         printk(KERN_WARNING"get the malloce for msg_buf fail!\n");
         goto out;	
    }
    
    msgs[0].buf[0] = reg_addr; 
    //printk("the reg addr is %x\n", reg_addr);    
    msgs[1].addr= client->addr;    
    msgs[1].flags = 1;  //������
    msgs[1].len = 1; 
    msgs[1].buf = kmalloc(1,GFP_KERNEL);
    if(!msgs[1].buf)
    {
         printk(KERN_WARNING"get the malloce for msg_buf fail!\n");
         goto out;	
    } 
    msgs[1].buf[0] = 0;  
       
    ret = i2c_transfer(client->adapter, msgs, 2);  
    if (ret != 2)  
        goto out;
    if(copy_to_user(buf, &(msgs[1].buf[0]) , count))  
        goto out;    
        
     /*�Ĵ�����ַ��1*/
     (*f_pos) += 1;
       
    return count;
    
    out:  
        kfree(msgs); 
        return -1;
	
}


/*�ַ��豸 write ����*/
static ssize_t gy50_write(struct file *fd, const char __user *buf, size_t count, loff_t *f_pos)
{
    int ret; 
    unsigned char *tmp; 
    struct i2c_msg msg;  
    struct i2c_client *client = (struct i2c_client*)fd->private_data; 
    
    tmp = kmalloc(count, GFP_KERNEL); 
    if(!tmp)
        goto out;

    if(copy_from_user(tmp, buf, count))
        goto out;
        
    msg.addr = client->addr;
    msg.flags = client->flags & (I2C_M_TEN | I2C_CLIENT_PEC);    
    msg.len = 2;  	
    msg.buf[0] = tmp[0];
    msg.buf[1] = tmp[1];
 
    printk("%x    %x \n", msg.buf[0], msg.buf[1]);
    ret = i2c_transfer(client->adapter, &msg, 1);  /*1:��Ϣ����nmasg*/

    if(ret !=1)
        goto out;
            
    msleep(1); 
   
    kfree(tmp);
    
    return count;
    
    out:  
        kfree(tmp);  
        return -1; 	
}


/*�ַ��豸 llseek ����*/
static loff_t gy50_lseek(struct file *fd , loff_t offset, int whence)
{
    loff_t newpos =0;
   
    switch (whence){
   	 case SEEK_SET :
   	   newpos = 0x28;  /*��ȡ�Ĵ����׵�ַ*/
   	   break;
   	   
   	 case SEEK_CUR :
   	   newpos = fd->f_pos + offset ;
   	   break; 
   	   
   	 case SEEK_END :   	 	
   	  newpos = 0x2D;  /*��ȡ�Ĵ���β��ַ*/
   	  break;
   	  
   	  default: 
   	  break;
   	 }
 	    
   	fd->f_pos = newpos;
   	    return newpos;    	
}


/*�ַ��豸 ioctl ����*/
static int gy50_ioctl(struct inode *inode, struct file *fd, unsigned int cmd, unsigned long arg)
{
    printk(KERN_WARNING"ioctl\n");
    return 0;	
}

/*�����ַ��豸����������*/
static struct file_operations gy50_i2c_fops = { 

    .owner = THIS_MODULE,
    .open = gy50_open,
    .read = gy50_read,
    .write = gy50_write,
    .unlocked_ioctl = gy50_ioctl,
    .llseek = gy50_lseek,
    .release = gy50_close,	
};

/*��ʼ���豸*/
static int __init gy50_i2c_init(void)
{  
    int res;
    
    /*1. ע���ַ��豸*/
    res = register_chrdev(I2C_CDEV_MAJOR, DEVICE_NAME, &gy50_i2c_fops);
    if(res)
    {
        printk(KERN_WARNING"%s unable to get  %d major number\n", DEVICE_NAME, I2C_CDEV_MAJOR);
        return -1;
        	
    }
    
    /*2. ��sysĿ¼�´�����*/
    gy50_class = class_create(THIS_MODULE, DEVICE_NAME);
    if (IS_ERR(gy50_class)) 
    {
	printk(KERN_ERR"couldn't create class\n");
        unregister_chrdev(I2C_CDEV_MAJOR, DEVICE_NAME); 
	return -1;
    }
    
    /*3. ע�ᵽi2c������*/
    return i2c_add_driver(&gy50_i2c_driver);	
}

/*�ر��豸*/
static void __exit gy50_i2c_exit(void)
{
    unregister_chrdev(I2C_CDEV_MAJOR, DEVICE_NAME); 
    device_destroy(gy50_class, 0);
    class_destroy(gy50_class); 
    i2c_del_driver(&gy50_i2c_driver);	
}

module_init(gy50_i2c_init);
module_exit(gy50_i2c_exit);

MODULE_DESCRIPTION("L3G4200D I2C driver");
MODULE_LICENSE("GPL");