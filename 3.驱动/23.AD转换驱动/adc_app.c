#include  <fcntl.h>
#include  <stdio.h>
#include  <sys/types.h>
#include  <sys/stat.h>
#include  <unistd.h>
#include  <stdlib.h>
#include  <sys/ioctl.h>
#include  <pthread.h>
#include  "adc_h.h"


/*������*/
pthread_mutex_t adc_mut;
int fd; 
unsigned int channel[2]={0 , 1};


/*�߳�1, �ɼ�����*/
void *light_collect()
{
    float light_v;
    
    while(1)
    { 
    	/*����,����.��Դ����*/
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


/*�߳�2, �ɼ�����Ũ��*/
void *smoke_collect()
{
    float smoke_v;
    
    while(1)
    {   
        /*����,����.��Դ����*/
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
    
    /*��ʼ�������� arg1: �� arg2: ����attr*/
    pthread_mutex_init(&adc_mut, NULL);
    
    fd = open("dev/adc", O_RDWR);
    if(fd < 0)
    {
        printf("open adc fail!\n");
        return -1;
    } 
    
    /*�����߳�*/
    ret = pthread_create(&tid1, NULL, light_collect, NULL);
    if(ret)
        perror("creat the tid1 fail!\n");
       
    usleep(5);
     
    ret = pthread_create(&tid2, NULL, smoke_collect, NULL);
    if(ret)
        perror("creat the tid2 fail!\n"); 
        
     
    /*�ȴ��߳̽���*/
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
   
   /*�˳�����*/  
    close(fd);
    exit(0);
}


/*���Ⱦ�������*/
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

/*����������*/
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

/*ad���ݲɼ�*/
float ad_data_collect(unsigned int channal)
{
    int ret = 0;
    unsigned short ad_data;
    float ad_val;
    
    ret = write (fd, &channal, 4);/*1.д������*/
    if(ret < 0)  
       printf("write channel fail!\n");	
               
    ret = read(fd, &ad_data, 2);/*2.��ȡ����*/
    if(ret < 0)
        printf("read the ad-data fail!\n");
        
    ad_val = (float)ad_data * 3.3 /1024.0; /*3.ת��Ϊ��ѹֵ*/ 
       
    ad_data = 0;
        
    return  ad_val;   
}

/*��������������*/
void beep_running(unsigned int rate)
{
    ioctl(fd, BEEP_ON); 
   // usleep(rate);
   // ioctl(fd, BEEP_OFF);  	
}



