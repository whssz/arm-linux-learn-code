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
   
  /*��ȡ��ֵ*/	
   key_t key;
   key = ftok("/home", 3);
  /*����/��ȡ�����ڴ�*/
   shmid = shmget(key, sizeof(struct shmbuf), IPC_CREAT);
  /*ӳ�乲���ڴ�*/
   p_buf = (struct shmbuf *)shmat(shmid, NULL, 0);
  /*ѭ��,��ȡ������д���ֵ*/
   while(running)
     {     
     	/*�ȴ����߰����ݶ���*/
     	while(1 == (p_buf->shmbuf_runner_flag)) 
     	   { 
     	      sleep(1);
     	      printf("wait read process!\n");	
     	   }
     	   /*��ȡ��׼�����ն˵�������Ϣ*/	  	 	
           fgets(p_buf->shmbuf_data, BUFFER_SIZE, stdin);	
           p_buf->shmbuf_runner_flag = 1;
           if( 0 == strncmp(p_buf->shmbuf_data, "end", 3) )
             {
                p_buf->shmbuf_runner_flag = 0;	
                printf("1\n");
                running = 0;
             }                  
      }
  
  /*���빲���ڴ�*/
  shmdt(p_buf);
  
  return 0;	
}