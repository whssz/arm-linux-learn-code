/*************************************
*�ļ���: add.c
*��������: 2016-9-20
*������: wuhao
*�ļ�˵��: �ں�ģ��,add!
*************************************/

#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");

int add(int a, int b)
{
  return a+b;	
}

static int add_init(void)
{
   
    return 0;
}

static void add_exit()
{
   	
}

EXPORT_SYMBOL(add);
module_init(add_init);
module_exit(add_exit);