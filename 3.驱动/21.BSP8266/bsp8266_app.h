#ifndef _BSP_8266_APP_
#define _BSP_8266_APP_

void set_cmd(unsigned char *cmd, unsigned char *ack);

unsigned char connect_cmd[80]= /*指定TCP服务器ip地址*/
{0x41 ,0x54 ,0x2B ,0x43 ,0x49 ,0x50 ,0x53 ,0x54 ,0x41 ,0x52 ,0x54, 
 0x3D ,0x22 ,0x54 ,0x43 ,0x50 ,0x22 ,0x2C ,0x22 ,0x31 ,0x38 ,0x30, 
 0x2E ,0x38 ,0x34 ,0x2E ,0x33 ,0x33 ,0x2E ,0x37 ,0x34 ,0x22 ,0x2C, 
 0x38 ,0x30 ,0x38 ,0x36, 0x0D, 0x0A}; 
 
 unsigned char wifi_name_cmd[50]=  /*指定wifi链接地址*/
{0x41,0x54,0x2B,0x43,0x57,0x4A,0x41,0x50,0x3D,0x22,0x77,0x75,0x68,
 0x61,0x6F,0x31,0x32,0x33,0x22,0x2C,0x22,0x77,0x75,0x68,0x61,0x6F,
 0x31,0x32,0x33,0x22,0x0D,0X0A};
#endif
