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
  /*循环读取数据*/
  while(running)
  {  
  	//printf("yes\n");
        /*打印共享内存*/
        while(1 == (p_buf->shmbuf_runner_flag))
        { 
           printf("the str is %s\n", p_buf->shmbuf_data); 
           p_buf->shmbuf_runner_flag = 0;           
        }
        if( 0 == strncmp(p_buf->shmbuf_data, "end", 3))
               running = 0;                   	
  }
  /*脱离共享内存*/
  shmdt(p_buf);
  
  /*删除共享内存*/
  shmctl(shmid, IPC_RMID, 0);
  
  return 0;	
}