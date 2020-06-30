#include <stdio.h>
#include <stdlib.h>

#define ADCCON    (*(volatile unsigned *)0x58000000)   //ADC control
#define ADCTSC    (*(volatile unsigned *)0x58000004)   //ADC touch screen control
#define ADCDAT0   (*(volatile unsigned *)0x5800000C) 

#define  BASIC_V   3.3/1024.0 
void adc_init(void)
{
 
    /*1.���ÿ��ƼĴ���*/
        /*1.1 ѡ��ͨ��*/
        /*1.2 ����ʱ��,1M*/
    ADCCON = (1<<14) | (49<<6) | (1<<3); 
}
     


void adc_data(void)
{
   float adc_data=0;
   float voltage;
   /*1.��������*/
   
   ADCCON |= (1<<0);

     
   /*2.�ȴ���ʼת��*/  
   while(ADCCON & 0x1);
 
   /*2.�ȴ��������*/
   while (!(ADCCON & (1<<15)));

   
   /*3.��ȡ����*/
   adc_data = (ADCDAT0 & 0x3ff);
 
   /*4.ת��Ϊ��ѹ*/
  // voltage = (float)(adc_data*3.3);
   //voltage = voltage / 1024;
  
   voltage = adc_data + BASIC_V;
   //printf("the ad_data is : %3.3f\r\n", voltage);	
}

