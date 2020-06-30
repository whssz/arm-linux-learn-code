#include <linux/init.h>
#include <linux/list.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");

struct person
{
  char *name;
  int age;
  struct list_head list;
};

struct list_head person_head;
struct person man1, man2, man3;
struct list_head *pos;
struct person *message;
static int mylist_init(void)
{  
    /*创建一个链表*/
    INIT_LIST_HEAD(&person_head);
    
    /*插入三个节点*/
    man1.name = "wu hao";
    man1.age = 20;
    list_add_tail(&man1.list, &person_head);
    
    man2.name = "gu liang jin";
    man2.age = 21;
    list_add_tail(&man2.list, &person_head);
    
    man3.name = "liu yi di ";
    man3.age = 20;
    list_add_tail(&man3.list, &person_head);
    
    /*遍历链表,并打印节点中的数据*/
    list_for_each(pos, &person_head)
    {
      message = list_entry(pos, struct person, list);  //指针域, 节点类型名, 节点中的指针域变量名 	
      printk("name is %s, age is %d\n", message->name, message->age);
    }
    
    return 0;	
}

void mylist_exit()
{
  list_del(&man1.list); 	
  list_del(&man2.list);
  list_del(&man3.list);  	
}

//加载函数
module_init(mylist_init);
//卸载函数
module_exit(mylist_exit);


     
     