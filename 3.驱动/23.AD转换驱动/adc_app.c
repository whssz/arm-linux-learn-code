#include  <fcntl.h>
#include  <stdio.h>
#include  <sys/types.h>
#include  <sys/stat.h>
#include  <unistd.h>
#include  <stdlib.h>
#include  <sys/ioctl.h>
#include  <pthread.h>
#include  "adc_h.h"


/*互斥锁*/
pthread_mutex_t adc_mut;
int fd; 
unsigned int channel[2]={0 , 1};


/*线程1, 采集亮度*/
void *light_collect()
{
    float light_v;
    
    while(1)
    { 
    	/*上锁,解锁.资源互斥*/
    	pthread_mutex_lock(&adc_mut);
        light_v = ad_data_collect(channel[0]);
        pthread_mutex_unlock(&adc_mut);
        
        bright_warn(light_v);
        
        printf("the light_v is: %1.2f\n", light_v);
        
        sleep(1);
        
        light_v = 0;
    }
    
    pthread_exit(NULL);
}


/*线程2, 采集烟雾浓度*/
void *smoke_collect()
{
    float smoke_v;
    
    while(1)
    {   
        /*上锁,解锁.资源互斥*/
    	pthread_mutex_lock(&adc_mut);
        smoke_v = ad_data_collect(channel[1]);
        pthread_mutex_unlock(&adc_mut);
        
        smoke_warn(smoke_v);
        
        printf("the smoke_v is: %1.2f\n", smoke_v);
        
        sleep(1);
        
        smoke_v = 0;
    }
    
    pthread_exit(NULL);
}


int main(void)
{      
    float light_v, smoke_v;
    pthread_t tid1, tid2;
    int ret;
    
    /*初始化互斥锁 arg1: 锁 arg2: 属性attr*/
    pthread_mutex_init(&adc_mut, NULL);
    
    fd = open("dev/adc", O_RDWR);
    if(fd < 0)
    {
        printf("open adc fail!\n");
        return -1;
    } 
    
    /*创建线程*/
    ret = pthread_create(&tid1, NULL, light_collect, NULL);
    if(ret)
        perror("creat the tid1 fail!\n");
       
    usleep(5);
     
    ret = pthread_create(&tid2, NULL, smoke_collect, NULL);
    if(ret)
        perror("creat the tid2 fail!\n"); 
        
     
    /*等待线程结束*/
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
   
   /*退出进程*/  
    close(fd);
    exit(0);
}


/*亮度警报函数*/
void bright_warn(float light_v)
{   
    if(light_v >= 2.3)
    {
        printf("***************************the bright is weak!\r\n");
        printf("***************************the led is running!\n");
        ioctl(fd, LED_ON);
    }  
    else
        ioctl(fd, LED_OFF);
}

/*烟雾警报函数*/
void smoke_warn(float smoke_v)
{   
    if(smoke_v >= 0.2)
    {
       printf("****************************the smoke is overproof!\r\n");
       printf("****************************the beep is alarm!\n");
       ioctl(fd, BEEP_ON);
    }
    else
        ioctl(fd, BEEP_OFF);  
   
}

/*ad数据采集*/
float ad_data_collect(unsigned int channal)
{
    int ret = 0;
    unsigned short ad_data;
    float ad_val;
    
    ret = write (fd, &channal, 4);/*1.写入数据*/
    if(ret < 0)  
       printf("write channel fail!\n");	
               
    ret = read(fd, &ad_data, 2);/*2.读取数据*/
    if(ret < 0)
        printf("read the ad-data fail!\n");
        
    ad_val = (float)ad_data * 3.3 /1024.0; /*3.转换为电压值*/ 
       
    ad_data = 0;
        
    return  ad_val;   
}

/*蜂鸣器起鸣函数*/
void beep_running(unsigned int rate)
{
    ioctl(fd, BEEP_ON); 
   // usleep(rate);
   // ioctl(fd, BEEP_OFF);  	
}



