/*************************************
*文件名: helloworld.c
*创建日期: 2016-9-20
*创建人: wuhao
*文件说明: 内核模块,helloworld!
*************************************/

#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("wuhao");
MODULE_DESCRIPTION("hello world");
MODULE_VERSION("v1.0");

extern add(int a, int b);
int a = 1;
char *p;
//module_param(a,int,S_IRUGO|S_IWUSR);
//module_param(p,charp,S_IRUGO|S_IWUSR);

static int hello_init(void)
{
   // printk(KERN_WARNING"hello world!\n");
   // printk("a = %d\n", a);
    //printk("p = %s\n", p);
    printk("add = %d\n", add(1,2));
    return 0;
}

static void hello_exit()
{
    printk(KERN_WARNING"good bye!\n");	
}


module_init(hello_init);
module_exit(hello_exit);
