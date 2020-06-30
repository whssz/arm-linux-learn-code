/************************************
*创建日期: 2016-11-7
*功能描述: BSP8266串口wifi的AT指令定义,
          作为station模式下的client
*文件名: BSP8266_cmd_list
***************************************/

/*发送命令类*/ 
#define   BSP_RST     "AT+RST\r\n"
#define   BSP_MODE    "AT+CWMODE=1\r\n"
#define   BSP_ENTER   "AT+CWJAP="wuhao123","wuhao123"\r\n"
#define   BSP_ONLY    "AT+CIPMUX=0\r\n"
#define   BSP_CONCT   "AT+CIPSTART=""TCP","180.84.33.74",8086\r\n"
#define   BSP_T_MODE  "AT+CIPMODE=1\r\n"
#define   BSP_STA_T   "AT+CIPSEND=20\r\n"
