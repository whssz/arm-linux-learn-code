/***************************************
*文件说明 ： 验证多个进程的同步， 信号量获取和释放的数值，按照顺序增加。
*创建日期 : 2016-12-13
*****************************************/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>



int main(void)
{
    pid_t pid1,pid2;
    key_t key;
    int sem_id;
    struct sembuf semops;
    
    /*获取IPC的键值*/
    key = ftok("/home",  1);
    if(key < 0)
        perror("get the key fail!\n");
     
     /*打开/创建信号量*/
     sem_id = semget(key, 1, IPC_CREAT);
     if(sem_id < 0)
         perror("get the sem_id fail!\n");
         
      /*设置/初始化信号量*/
      semctl(sem_id, 0, SETVAL, 0);
      
      /*创建子进程*/
      pid1 = fork();
      if(pid1 < 0)
          perror("pid1 fork fail!\n");    
      else if(0 == pid1)
      {
           /*获取信号量*/
           semops.sem_num = 0;
           semops.sem_op = -2;
           semops.sem_flg = SEM_UNDO;
           semop(sem_id, &semops, 1);
           
           printf("C task is finished!\n"); 
            
           /*释放信号量,为1*/
           semops.sem_num = 0;
           semops.sem_op = 1;
           semops.sem_flg = SEM_UNDO;
           semop(sem_id, &semops, 1);  
      }       
      else
      {
         pid2 = fork();
         if(pid2 < 0)
             perror("pid2 fork fail!\n");
         else if(0 == pid2)
         {
             /*获取信号量*/
             semops.sem_num = 0;
             semops.sem_op = -1;
             semops.sem_flg = SEM_UNDO;
             semop(sem_id, &semops, 1);   
             
             sleep(5);
             printf("B task is finished!\n");
             
             /*释放信号量*/
             semops.sem_num = 0;
             semops.sem_op = 2;
             semops.sem_flg = SEM_UNDO;
             semop(sem_id, &semops, 1);  
         }
         
         else
         {   
             sleep(10);
             printf("A task is finished!\n");
             /*释放信号量*/
             semops.sem_num = 0;
             semops.sem_op = 1;
             semops.sem_flg = SEM_UNDO;
             semop(sem_id, &semops, 1); 
             
             while( wait(NULL) != -1 ); /*父进程等待 所有进程退出，再退出*/           
         }
                
      }  
          
     exit(0);           
}


















