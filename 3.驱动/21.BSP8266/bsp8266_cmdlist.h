/************************************
*��������: 2016-11-7
*��������: BSP8266����wifi��ATָ���,
          ��Ϊstationģʽ�µ�client
*�ļ���: BSP8266_cmd_list
***************************************/

/*����������*/ 
#define   BSP_RST     "AT+RST\r\n"
#define   BSP_MODE    "AT+CWMODE=1\r\n"
#define   BSP_ENTER   "AT+CWJAP="wuhao123","wuhao123"\r\n"
#define   BSP_ONLY    "AT+CIPMUX=0\r\n"
#define   BSP_CONCT   "AT+CIPSTART=""TCP","180.84.33.74",8086\r\n"
#define   BSP_T_MODE  "AT+CIPMODE=1\r\n"
#define   BSP_STA_T   "AT+CIPSEND=20\r\n"
