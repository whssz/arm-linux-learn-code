#include "vsprintf.h" //必须包含头文件

unsigned char outbuf[1024];
unsigned char inbuf[1024];

int printf(const char* fmt,...)
{
	unsigned int i;
	va_list args;	       //定义变参列表                 
 	va_start(args,fmt);  //fmt转化为变参列表
	vsprintf((char*)outbuf,fmt,args); // 变参列表转化为字符串
	va_end(); //转化结束
		
  //2.打印字符到串口
  for(i=0;i<strlen((const char*)outbuf);i++)
  {
  	send_data(outbuf[i]);
  	
  }
	return i;
}


int scanf(const char* fmt, ...)
{
	 unsigned char c;	
	 int i = 0;
	 va_list args;
	 while(1)
	 { 
	   c = receive_data();
	   if((c == 0x0d) || (c ==0x0a))  //如果采集到回车换行符，就算做这个数据输入完
	    {
	        inbuf[i] = '\n';
	    	break;	
	    }	
	   else
	    {
	   	inbuf[i++] = c;	 
	    }
	 }
    	 //格式转换   	 
    	                
 	va_start(args, fmt);
	vsscanf((char *)inbuf,fmt,args);
	va_end(args);
    	 //读取数据
    	 return i; //返回数据个数   	 
}