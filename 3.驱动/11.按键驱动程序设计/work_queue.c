#include <linux/module.h>
#include <linux/init.h>



//struct workqueue_struct *work_queue;
static struct work_struct *work1;
static struct work_struct *work2;

static void work1_func(struct work_struct *work)
{
	printk("the work1!\n");
}

static void work2_func(struct work_struct *work)
{
	printk("the work2!\n");
}

static int work_queue_init(void)
{    
    /*创建工作队列*/
   // work_queue = create_workqueue("my_queue");
         
    /*创建工作*/
    //为工作描述分配一段内存
    work1 = kmalloc(sizeof(struct work_struct), GFP_KERNEL);
    INIT_WORK(work1, work1_func);
    /*挂载工作*/
    //queue_work(work_queue, work1);
    schedule_work(work1);
   
   
    /*创建工作*/
    //为工作描述分配一段内存
    work2 = kmalloc(sizeof(struct work_struct), GFP_KERNEL);
    INIT_WORK(work2, work2_func);
    /*挂载工作*/
   // queue_work(work_queue, work2);
    schedule_work(work2);
    
    
    return 0;	
}


static void work_queue_exit()
{
    	
}

MODULE_LICENSE("GPL");
module_init(work_queue_init);
module_exit(work_queue_exit);