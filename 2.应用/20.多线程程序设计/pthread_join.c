/***************************
*文件描述： 验证pthread_join  是否等待任意一个线程结束结束
* 验证结果： pthread_join 需要等待所有的线程结束才会结束进程
*****************************/

#include  <unistd.h>
#include  <sys/types.h>
#include  <pthread.h>
#include  <stdio.h>

void * tid1_func(void)
{
    while(1)
    {
        printf("tid1!\n");
        sleep(2);
    }
    
    pthread_exit(NULL);
}

void * tid2_func(void)
{
   while(1)
    {
        printf("tid2!\n");
        sleep(2);
    }
    
    pthread_exit(NULL);
}


int main(void)
{
    pthread_t tid1, tid2;
    pthread_create(&tid1, NULL, tid1_func, NULL);
    usleep(200);
    pthread_create(&tid2, NULL, tid2_func, NULL);
    usleep(200);
    
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    printf("over!\n");
}

