#include  <stdio.h>
#include  <unistd.h>
#include  <string.h>
#include  <stdlib.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include  <sys/types.h>

typedef struct Student
{
    char *name;
    int age;
    int id;  
}Stu,*P_stu;

    
int main(void)
{
    pid_t pid1;
      
    /*获取键值*/
    key_t key=0;
    key = ftok("/home", 1);  
    P_stu student;     
    /*创建 共享内存*/      
    int shmid = 0;   
    shmid = shmget(key, sizeof(Stu), IPC_CREAT);  
    printf("%d\n", sizeof(Stu)); 
    
    /*创建子进程*/
    pid1 = fork();
    if(pid1 < 0)
        perror("creat child process fail!\n");  
    else if(0 == pid1)
    {   
            
         /*映射共享内存*/
         student = (P_stu)shmat(shmid, NULL, 0);
         
         /*写入数据*/                
         student->name ="gu liang jin";  /*gu liang jin 字符串是放在常量区，结构体分配的只是指针，指向这段常量区*/        
         student->age = 21;
         student->id = 5;  
      
         /*分离共享内存*/
         shmdt(student);
                 
         exit(0);
    }   
    else
    {      
         /*等待子进程退出*/
         wait(NULL); 
          
         /*映射共享内存*/        
         student = (P_stu)shmat(shmid, NULL, 0);
        
         /*读取数据*/
         printf("the name : %s\n",  student->name);
         printf("the age : %d\n", student->age);
         printf("the student ID : %d\n", student->id);
                
         /*分离共享内存*/  
          shmdt(student);
          
          /*删除共享内存*/
          shmctl(shmid, IPC_RMID, 0);
          
          exit(0);
    } 
}







