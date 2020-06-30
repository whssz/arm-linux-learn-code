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

/*分配一个blk_dev结构,包含块设备的信息*/
struct blk_dev{  
    int size;
    struct request_queue *queue;
    struct gendisk *gd;
    u8 *data;
};


struct blk_dev *dev;
;
  	

/*gendisk 文件描述结构*/
static struct block_device_operations blk_fops={
	.owner = THIS_MODULE,
};


/*读写处理函数*/
void req_transfer(struct blk_dev *dev, unsigned long sector, unsigned long nsect, char *buf, int write)
{
    unsigned long offset = sector*sect_size;
    unsigned long nbyte = nsect*sect_size;
    
    if((offset+nbyte) > dev->size)
    {
        printk(KERN_WARNING"Beyond-end the block!(%ld, %ld)\n", offset, nbyte);
        nbyte = dev->size - offset;	
    }
    if(write)
        memcpy((dev->data)+offset, buf, nbyte);
    else
        memcpy(buf, (dev->data)+offset, nbyte);   
}
/*读写请求队列函数*/
static void blk_request(struct request_queue *q)
{ 
    struct request *req;
    
    /*从队列里面获取一个请求*/
    req = blk_fetch_request(q);
    
    /*处理请求*/
    while(req)
    {  
        struct blk_dev *dev = dev->gd->private_data;
        
        /*分别获取: 起始扇区, 操作扇区个数, 读写方向*/
        req_transfer(dev, blk_rq_pos(req), blk_rq_cur_sectors(req), req->buffer, rq_data_dir(req)); 
         
        /*判断队列里面是否还有请求*/ 
        if(!__blk_end_request_cur(req, 0))
            req = blk_fetch_request(q);        	
    }    	
}

/*初始化函数*/
static int __init blk_init(void)
{    
    /*分配初始化blk_dev结构*/
    dev = kmalloc(sizeof(struct blk_dev), GFP_KERNEL);
    if(!dev)
    {
        printk(KERN_WARNING"kmalloc the dev fail!\n");
        kfree(dev);
        return -EFAULT;	
    }
    
    dev->size = nsectors*sect_size;
    dev->data = vmalloc(dev->size);
    if (dev->data == NULL) 
    {
        printk (KERN_WARNING "vmalloc failure.\n");
        return -EFAULT;
    }
    
    /*1.注册块设备*/
    major = register_blkdev(0, "my_blk");
    if((major)<0)
    {
        printk(KERN_WARNING"add my_blk fail!'n");
        unregister_blkdev(major, "my_blk");
        return -EBUSY;	
    }
    
    /*2.初始化工作队列*/
    dev->queue = blk_init_queue(blk_request, NULL);    
    if(!(dev->queue))
    {
        printk(KERN_WARNING"get the request queue fail!\n");	
    }	    
    
    /*3.设置扇区尺寸, */
    blk_queue_logical_block_size(dev->queue, sect_size);
    
    /*4.分配gendisk结构,并初始化, 1: 该驱动管理1个块设备*/
    dev->gd = alloc_disk(1); 
    if(!(dev->gd))
    {
        printk(KERN_WARNING"get the gendisk fail!\n");
        del_gendisk(dev->gd);  /*注销gendisk结构*/
        return -EFAULT;	
    }
    
    dev->gd->major = major;
    dev->gd->first_minor = 0;
    dev->gd->minors = 1;
    dev->gd->fops = &blk_fops;
    dev->gd->queue = dev->queue;
    dev->gd->private_data = dev;
    sprintf(dev->gd->disk_name, "blk_dev%d", 0); /*初始化disk_name*/
    set_capacity(dev->gd, nsectors*(sect_size/512)); /*capacity, 表示扇区为512字节时,块设备能够包含的扇区数,不能直接赋值*/
    
    add_disk(dev->gd); /*注册到内核*/
    return 0;	
}


static void __exit blk_exit(void)
{
     del_gendisk(dev->gd);   
     put_disk(dev->gd);
     blk_cleanup_queue(dev->queue);
      	
}


module_init(blk_init);
module_exit(blk_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("WUHAO");
