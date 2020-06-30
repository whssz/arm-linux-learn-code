#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 2048

int main(void)
{
   int shmid;	
   struct shmbuf 
   {
    int shmbuf_runner_flag ;
    char shmbuf_data[BUFFER_SIZE];	
   };
   struct shmbuf *p_buf;
   int running = 1;
   
  /*获取键值*/	
   key_t key;
   key = ftok("/home", 3);
  /*创建/获取共享内存*/
   shmid = shmget(key, sizeof(struct shmbuf), IPC_CREAT);
  /*映射共享内存*/
   p_buf = (struct shmbuf *)shmat(shmid, NULL, 0);
  /*循环,获取键盘所写入的值*/
   while(running)
     {     
     	/*等待读者把数据读走*/
     	while(1 == (p_buf->shmbuf_runner_flag)) 
     	   { 
     	      sleep(1);
     	      printf("wait read process!\n");	
     	   }
     	   /*获取标准输入终端的输入信息*/	  	 	
           fgets(p_buf->shmbuf_data, BUFFER_SIZE, stdin);	
           p_buf->shmbuf_runner_flag = 1;
           if( 0 == strncmp(p_buf->shmbuf_data, "end", 3) )
             {
                p_buf->shmbuf_runner_flag = 0;	
                printf("1\n");
                running = 0;
             }                  
      }
  
  /*脱离共享内存*/
  shmdt(p_buf);
  
  return 0;	
}