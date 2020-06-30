#ifndef  _ADC_H_H_
#define  _ADC_H_H_

#define  BEEP_MAGIC  'B'
#define  BEEP_ON     _IO(BEEP_MAGIC, 0) 
#define  BEEP_OFF    _IO(BEEP_MAGIC, 1)

#define LED_MAGIC  'L'
#define LED_ON       _IO(LED_MAGIC, 0)
#define LED_OFF      _IO(LED_MAGIC, 1)

void bright_warn(float );
void smoke_warn(float );
float ad_data_collect(unsigned int );
void beep_running(unsigned int rate);
int led_beep_hardinit(void);

#endif