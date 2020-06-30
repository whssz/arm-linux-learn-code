#include "vsprintf.h" //�������ͷ�ļ�

unsigned char outbuf[1024];
unsigned char inbuf[1024];

int printf(const char* fmt,...)
{
	unsigned int i;
	va_list args;	       //�������б�                 
 	va_start(args,fmt);  //fmtת��Ϊ����б�
	vsprintf((char*)outbuf,fmt,args); // ����б�ת��Ϊ�ַ���
	va_end(); //ת������
		
  //2.��ӡ�ַ�������
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
	   if((c == 0x0d) || (c ==0x0a))  //����ɼ����س����з����������������������
	    {
	        inbuf[i] = '\n';
	    	break;	
	    }	
	   else
	    {
	   	inbuf[i++] = c;	 
	    }
	 }
    	 //��ʽת��   	 
    	                
 	va_start(args, fmt);
	vsscanf((char *)inbuf,fmt,args);
	va_end(args);
    	 //��ȡ����
    	 return i; //�������ݸ���   	 
}