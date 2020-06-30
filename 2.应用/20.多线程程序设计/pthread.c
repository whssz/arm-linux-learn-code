#include <stdio.h>
#include <pthread.h>

unsigned char num = 0;
pthread_mutex_t mutex;

void *work1()
{
  unsigned char i =0;
  printf("work1 is working!\n");
  for(i=0; i<10; i++)
  {
     pthread_mutex_lock(&mutex);
     num ++ ;   
     printf("work1 num is %d\n", num);	
     pthread_mutex_unlock(&mutex);
     sleep(1);
  }	
  pthread_exit(NULL);
}

void *work2()
{
  unsigned char i =0;
  printf("work2 is working!\n");
  for(i=0; i<10; i++)
  {
     pthread_mutex_lock(&mutex);
     num ++ ;  
     printf("work2 num is %d\n", num);	
     pthread_mutex_unlock(&mutex);
     sleep(1);     
  }
  pthread_exit(NULL);		
}

int main(void)
{  
  pthread_t fd1,fd2;
  /*��ʼ��������*/
   pthread_mutex_init(&mutex, NULL);
  /*�����߳�1*/
   pthread_create(&fd1, NULL, work1, NULL);
  /*�����߳�2*/
   pthread_create(&fd2, NULL, work2, NULL);
  /*�ȴ��߳�1�˳�*/
  pthread_join(fd1, NULL);
  /*�ȴ��߳�2�˳�*/	
  pthread_join(fd2, NULL);
  	
  return 0;	
}