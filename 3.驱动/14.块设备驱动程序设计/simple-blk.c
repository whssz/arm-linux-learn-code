#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>

#include <linux/sched.h>
#include <linux/kernel.h> /* printk() */
#include <linux/slab.h>   /* kmalloc() */
#include <linux/fs.h>   /* everything... */
#include <linux/errno.h> /* error codes */
#include <linux/timer.h>
#include <linux/types.h> /* size_t */
#include <linux/fcntl.h> /* O_ACCMODE */
#include <linux/hdreg.h> /* HDIO_GETGEO */
#include <linux/kdev_t.h>
#include <linux/vmalloc.h>
#include <linux/genhd.h>
#include <linux/blkdev.h>
#include <linux/buffer_head.h> /* invalidate_bdev */
#include <linux/bio.h>

unsigned int major = 0;
unsigned short sect_size = 512;
unsigned short nsectors = 1024;

/*定义gendisk的操作函数集*/
static struct block_device_operations blk_ops={	
    .owner = THIS_MODULE,       	
};

/*自行定义的结构体,描述一个块结构*/
struct blk_dev
{  
    int size; //大小
    u8 *data; //数据
    struct request_queue *queue;  //请求队列
    struct gendisk *gd;   //linux内核块描述结构
};

/*定义结构体变量*/
struct blk_dev *dev;


/*扇区处理函数*/
void blk_transfer(struct blk_dev *dev, unsigned long sector, unsigned long nsect, char *buffer, int write)
{
    unsigned long offset = sector*sect_size; //因为还有扇区0. 对第2个扇区操作,2*512 刚好是起始地址
    unsigned long nbytes = nsect*sect_size;
    
    if((offset+nbytes) > dev->size)
    {
        printk(KERN_NOTICE"Beyond-end write (%ld %ld)\n", offset,nbytes);
        return;	
    }
    
    if(write)
        memcpy(dev->data+offset, buffer, nbytes);
    else
        memcpy(buffer, dev->data+offset, nbytes);  
}
/*队列处理函数*/
static void blk_request(struct request_queue *q)
{    
    struct request *req;
    req = blk_fetch_request(q);  //接收到在队列里面提取出来的那条队列
    while(req != NULL)  
    {    //处理队列请求. dev, 请求操作的起始扇区,操作扇区数,数据放入什么地方,是读还是写
         blk_transfer(dev, blk_rq_pos(req), blk_rq_cur_sectors(req), req->buffer, rq_data_dir(req));
                                
         if(!__blk_end_request_cur(req, 0)) //判断是否是最后一个请求
         {
             req = blk_fetch_request(q); 	
         } 
    }
    	
}


/*初始化dev结构体和dev->gd结构体*/
static void setup_device(void)
{   
	
    //初始化dev的其他变量
    dev->size = sect_size*nsectors;
    dev->data = vmalloc(dev->size);
    if((dev->data) == NULL)
    {
        printk("vmalloc get fail!\n");
        return;	
    }
    
    //初始化队列, 接收队列处理函数,自旋锁(NULL)
    dev->queue = blk_init_queue(blk_request, NULL);
    if (dev->queue == NULL)
        goto out_vfree; 
    //设置扇区大小
    blk_queue_logical_block_size(dev->queue, sect_size);
    
    //分配设备结构, arg:次设备号的数量 
    dev->gd = alloc_disk(1); 
    if (! dev->gd) 
    {
       printk (KERN_NOTICE "alloc_disk failure\n");
       goto out_vfree;
    }    
    
    //初始化 gendisk 这个结构体
    dev->gd->major = major;
    dev->gd->first_minor = 0; //次设备号的起始设备号
     //dev->gd->minors = 1;  //有待修改,可行
    dev->gd->fops = &blk_ops;    
    dev->gd->queue = dev->queue;
    dev->gd->private_data = dev;
    sprintf(dev->gd->disk_name, "simp_blk%d", 0);
    set_capacity(dev->gd, nsectors*(sect_size/sect_size));
    
    //注册块设备
    add_disk(dev->gd);      
    return;
    
out_vfree:
    if (dev->data)
    vfree(dev->data);                   
}

/*模块入口函数*/
static int __init blk_init(void)
{   
	   
    //注册设备驱动
    major = register_blkdev(0, "blk"); //主设备号,设备名称
    if(major <= 0) 
    {     
        printk("fail to get major number!\n");
        return -EBUSY;	
    } 
      
    //分配内存给结构体dev,试试改为vmalloc,分配内核内存最常用的GFP_KERNEL
    dev = kmalloc(sizeof(struct blk_dev), GFP_KERNEL);  //vmalloc和kmalloc都可以,证实可行
    if(dev == NULL)
        goto out_unregister;
    
    //初始化
    setup_device();
    
    return 0;	
    
out_unregister:
    unregister_blkdev(major, "sbd");
    return -ENOMEM;
}

static void blk_exit(void)
{     
   
    if (dev->gd) {
    del_gendisk(dev->gd);//注销gd
    put_disk(dev->gd);
   }
   if (dev->queue)
    blk_cleanup_queue(dev->queue); //清除队列
   if (dev->data)
    vfree(dev->data);//释放vfree

   unregister_blkdev(major, "blk");  //注销设备驱动    	
}

module_init(blk_init);
module_exit(blk_exit);

MODULE_LICENSE("GPL");  //申明是任意版本的GNU