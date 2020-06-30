#include  <stdio.h>
#include  <pthread.h>
#include  <unistd.h>

pthread_mutex_t mut; 
unsigned int num=0;
void  * tid1_func()
{ 
    int a=0;
	//pthread_mutex_lock(&mut);
	pthread_mutex_lock(&mut);
	num++;
	pthread_mutex_unlock(&mut);
	
	printf("tid1 num = %d\n", num);
	
}

void  * tid2_func()
{
   int a=0;
   
   pthread_mutex_lock(&mut);
   sleep(1);
   num++;
   pthread_mutex_unlock(&mut);
   
   printf("tid2 num = %d\n", num);
}

int main(void)
{
    pthread_t tid1, tid2;
    int ret;
    
    /*初始化互斥锁*/
    pthread_mutex_init(&mut, NULL);
    
    /*创建线程1*/
    ret = pthread_create(&tid1, NULL, tid1_func, NULL);
    if(ret)
        perror("create tid1 fail!\n");
        
    /*创建线程2*/
    ret = pthread_create(&tid2, NULL, tid2_func, NULL);
    if(ret)
        perror("create tid2 fail!\n");
        
    /*等待vid1退出*/
    pthread_join(tid1, NULL);
    
    /*等待vid2退出*/
    pthread_join(tid2, NULL);
    
    /*进程退出*/
    exit(0);
}
