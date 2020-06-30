#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

/*初始化条件变量*/
pthread_cond_t my_cond = PTHREAD_COND_INITIALIZER;
/*定义互斥锁*/
pthread_mutex_t my_mutex;

void *func1()
{
    printf("A plan success!\n");
         
    sleep(2);
    
    printf("B plan success!\n");
    
    //pthread_mutex_lock(&my_mutex); 
    printf("1\n");
     /*通知条件成熟*/
    pthread_cond_signal(&my_cond);   
  //  pthread_mutex_unlock(&my_mutex); 
    
      
    pthread_exit(NULL);
    
}

void *func2()
{
    int i = 0;
  
    printf("2\n");
    pthread_mutex_lock(&my_mutex); 
    /*等待条件成熟*/
    pthread_cond_wait(&my_cond, &my_mutex);   
    pthread_mutex_unlock(&my_mutex);//上锁
    
    printf("C pian success!\n");
    
    pthread_exit(NULL);
}

int main(void)
{
    pthread_t tid1,tid2;
    int ret;
    
    /*初始化互斥锁*/
    pthread_mutex_init(&my_mutex, NULL);
    
    /*创建线程*/
    ret = pthread_create(&tid1, NULL, func1, NULL);
    if(ret < 0)
        perror("creat tid1 fail!\n");
        
    ret = pthread_create(&tid2, NULL, func2, NULL);
    if(ret < 0)
        perror("creat tid2 fail!\n");
        
    /*等待线程结束*/
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    
    exit(0);  
}








