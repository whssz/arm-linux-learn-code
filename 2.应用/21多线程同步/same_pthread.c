#include <pthread.h>
#include <stdio.h>

typedef unsigned char u8;
typedef int u32;

pthread_cond_t  cond_ready = PTHREAD_COND_INITIALIZER;  //������������
pthread_t pthread[2];
pthread_mutex_t mux;
u8 num = 0;

void *studentA()
{ 
    u8 j;	
   /*Aͬѧɨ��*/
   //sleep(3);
   for(j=0; j<5; j++)
   {
     pthread_mutex_lock(&mux);
     num++;
     printf("the num is %d\n", num);
     
     if(num >= 5)
     {    	
        printf("studentA has finished work!\n");   
         
        pthread_cond_signal(&cond_ready); // ֪ͨ��������      
     }
     
     pthread_mutex_unlock(&mux); 
     sleep(1);
   }
   /*�˳��߳�*/	
   pthread_exit(NULL);
}

void *studentB()
{
   /*�ȴ���ʱ���ϵ�*/
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
    pthread_mutex_lock(&mux);//����
    if(num <5)
       pthread_cond_wait(&cond_ready, &mux); //�ȴ���������,�ȴ��ǻ����,����ʱ������
    
    num = 0 ;
    printf("the num is %d\n", num);
    printf("studentB has finished work!\n");
    
    pthread_mutex_unlock(&mux); //����	    
    
   /*�˳��߳�*/	
    pthread_exit(NULL);
}



int main(void)
{ 
  /*��ʼ��������*/
  pthread_mutex_init(&mux, NULL);
  /*�����߳�A*/
  pthread_create(&pthread[0], NULL, studentA, NULL);
  /*�����߳�B*/
  pthread_create(&pthread[1], NULL, studentB, NULL);
  /*�ȴ��߳�A����*/
  pthread_join(pthread[0], NULL);
  /*�ȴ��߳�B����*/
  pthread_join(pthread[1], NULL);
  
  return 0;	
}
