#include <stdio.h>

#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

#define container_of(ptr, type, member) ({          \
	const typeof(((type *)0)->member)*__mptr = (ptr);    \
		     (type *)((char *)__mptr - offsetof(type, member)); })
		     
#define list_entry(ptr, type, member) \
	container_of(ptr, type, member)	
	
	     

struct list_head
{
    struct list_head *next;
    struct list_head *prev;	
};

struct stu
{
    char *name;
    int age;
    struct list_head list;
};



struct stu stu1;
struct list_head head;
struct stu *new_data;

int main(void)
{  
    /*创建一个链表*/
    head.next = &head;
    head.prev = &head;
    /*插入一个链表*/
    stu1.name = "wuhao";
    stu1.age = 20;
    head.prev = &stu1.list;
    stu1.list.next = &head;
    stu1.list.prev = &head;
    head.next = &stu1.list;
      
    /*读取链表的值*/
    new_data = list_entry( &stu1.list, struct stu, list);
    printf("%s, %d\n", new_data->name,  new_data->age);
    return 0;	
}