#include <pthread.h>
#include <stdio.h>

typedef unsigned char u8;
typedef int u32;

pthread_cond_t  cond_ready = PTHREAD_COND_INITIALIZER;  //定义条件变量
pthread_t pthread[2];
pthread_mutex_t mux;
u8 num = 0;

void *studentA()
{ 
    u8 j;	
   /*A同学扫地*/
   //sleep(3);
   for(j=0; j<5; j++)
   {
     pthread_mutex_lock(&mux);
     num++;
     printf("the num is %d\n", num);
     
     if(num >= 5)
     {    	
        printf("studentA has finished work!\n");   
         
        pthread_cond_signal(&cond_ready); // 通知条件成熟      
     }
     
     pthread_mutex_unlock(&mux); 
     sleep(1);
   }
   /*退出线程*/	
   pthread_exit(NULL);
}

void *studentB()
{
   /*等待到时间拖地*/
   /*while(1)
   {    
      if(num >= 5)
      {  
      	 pthread_mutex_lock(&mux);
      	 
         num = 0;
         printf("the num is %d\n", num);
         printf("studentB has finished work!\n");
         
         pthread_mutex_unlock(&mux);	
         break;
      }   	
   }*/
    printf("yes\n");
    sleep(10);  
    pthread_mutex_lock(&mux);//上锁
    if(num <5)
       pthread_cond_wait(&cond_ready, &mux); //等待条件成熟,等待是会解锁,唤醒时会上锁
    
    num = 0 ;
    printf("the num is %d\n", num);
    printf("studentB has finished work!\n");
    
    pthread_mutex_unlock(&mux); //解锁	    
    
   /*退出线程*/	
    pthread_exit(NULL);
}



int main(void)
{ 
  /*初始化互斥锁*/
  pthread_mutex_init(&mux, NULL);
  /*创建线程A*/
  pthread_create(&pthread[0], NULL, studentA, NULL);
  /*创建线程B*/
  pthread_create(&pthread[1], NULL, studentB, NULL);
  /*等待线程A结束*/
  pthread_join(pthread[0], NULL);
  /*等待线程B结束*/
  pthread_join(pthread[1], NULL);
  
  return 0;	
}
