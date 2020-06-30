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

/*����gendisk�Ĳ���������*/
static struct block_device_operations blk_ops={	
    .owner = THIS_MODULE,       	
};

/*���ж���Ľṹ��,����һ����ṹ*/
struct blk_dev
{  
    int size; //��С
    u8 *data; //����
    struct request_queue *queue;  //�������
    struct gendisk *gd;   //linux�ں˿������ṹ
};

/*����ṹ�����*/
struct blk_dev *dev;


/*����������*/
void blk_transfer(struct blk_dev *dev, unsigned long sector, unsigned long nsect, char *buffer, int write)
{
    unsigned long offset = sector*sect_size; //��Ϊ��������0. �Ե�2����������,2*512 �պ�����ʼ��ַ
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
/*���д�����*/
static void blk_request(struct request_queue *q)
{    
    struct request *req;
    req = blk_fetch_request(q);  //���յ��ڶ���������ȡ��������������
    while(req != NULL)  
    {    //�����������. dev, �����������ʼ����,����������,���ݷ���ʲô�ط�,�Ƕ�����д
         blk_transfer(dev, blk_rq_pos(req), blk_rq_cur_sectors(req), req->buffer, rq_data_dir(req));
                                
         if(!__blk_end_request_cur(req, 0)) //�ж��Ƿ������һ������
         {
             req = blk_fetch_request(q); 	
         } 
    }
    	
}


/*��ʼ��dev�ṹ���dev->gd�ṹ��*/
static void setup_device(void)
{   
	
    //��ʼ��dev����������
    dev->size = sect_size*nsectors;
    dev->data = vmalloc(dev->size);
    if((dev->data) == NULL)
    {
        printk("vmalloc get fail!\n");
        return;	
    }
    
    //��ʼ������, ���ն��д�����,������(NULL)
    dev->queue = blk_init_queue(blk_request, NULL);
    if (dev->queue == NULL)
        goto out_vfree; 
    //����������С
    blk_queue_logical_block_size(dev->queue, sect_size);
    
    //�����豸�ṹ, arg:���豸�ŵ����� 
    dev->gd = alloc_disk(1); 
    if (! dev->gd) 
    {
       printk (KERN_NOTICE "alloc_disk failure\n");
       goto out_vfree;
    }    
    
    //��ʼ�� gendisk ����ṹ��
    dev->gd->major = major;
    dev->gd->first_minor = 0; //���豸�ŵ���ʼ�豸��
     //dev->gd->minors = 1;  //�д��޸�,����
    dev->gd->fops = &blk_ops;    
    dev->gd->queue = dev->queue;
    dev->gd->private_data = dev;
    sprintf(dev->gd->disk_name, "simp_blk%d", 0);
    set_capacity(dev->gd, nsectors*(sect_size/sect_size));
    
    //ע����豸
    add_disk(dev->gd);      
    return;
    
out_vfree:
    if (dev->data)
    vfree(dev->data);                   
}

/*ģ����ں���*/
static int __init blk_init(void)
{   
	   
    //ע���豸����
    major = register_blkdev(0, "blk"); //���豸��,�豸����
    if(major <= 0) 
    {     
        printk("fail to get major number!\n");
        return -EBUSY;	
    } 
      
    //�����ڴ���ṹ��dev,���Ը�Ϊvmalloc,�����ں��ڴ���õ�GFP_KERNEL
    dev = kmalloc(sizeof(struct blk_dev), GFP_KERNEL);  //vmalloc��kmalloc������,֤ʵ����
    if(dev == NULL)
        goto out_unregister;
    
    //��ʼ��
    setup_device();
    
    return 0;	
    
out_unregister:
    unregister_blkdev(major, "sbd");
    return -ENOMEM;
}

static void blk_exit(void)
{     
   
    if (dev->gd) {
    del_gendisk(dev->gd);//ע��gd
    put_disk(dev->gd);
   }
   if (dev->queue)
    blk_cleanup_queue(dev->queue); //�������
   if (dev->data)
    vfree(dev->data);//�ͷ�vfree

   unregister_blkdev(major, "blk");  //ע���豸����    	
}

module_init(blk_init);
module_exit(blk_exit);

MODULE_LICENSE("GPL");  //����������汾��GNU