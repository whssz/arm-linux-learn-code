#define  LED_MAGIC  'l' 
#define  LED_ON   _IO(LED_MAGIC, 0) 
#define  LED_OFF  _IO(LED_MAGIC, 1) 

#define BEEP_MAGIC  'b'
#define BEEP_ON   _IO(BEEP_MAGIC, 0) 
#define BEEP_OFF  _IO(BEEP_MAGIC, 1)