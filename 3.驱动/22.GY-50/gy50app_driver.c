#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/types.h>
#include <sys/ioctl.h>
#include <string.h>
#include <stdlib.h>
#include "gy_50.h"

int fd;
struct i2c_rdwr_ioctl_data gy50_data; /*������Ϣ�ṹ��*/
unsigned char x_y_z[6];

int main(void)
{     
   
    char temperature=0;
    /*1.��i2c�豸*/
    fd = open("/dev/i2c-0", O_RDWR);
    if(fd < 0)
       printf("open the i2c_bus fail!\n");
       
    /*����i2c_msg*/    
    gy50_data.msgs = (struct i2c_msg *)malloc(2*sizeof(struct i2c_msg));
    if(NULL ==  gy50_data.msgs)
        printf("get the msgsbuf fail!\n");
   

    /*2.��ʼ��L3G4200*/
    Single_WriteL3G4200D(CTRL_REG1, 0x0f); /*ʹ�����*/
    usleep(200);  
    Single_WriteL3G4200D(CTRL_REG2, 0x00);   
    usleep(200); 
    Single_WriteL3G4200D(CTRL_REG3, 0x08); 
    usleep(200);   
    Single_WriteL3G4200D(CTRL_REG4, 0x00);   /*250*/
    usleep(200); 
    Single_WriteL3G4200D(CTRL_REG5, 0x00);
    usleep(200); 
    
   
    /*3. ��ȡ�¶�*/
   
    
  
    while(1)
    {   
       temperature = Single_ReadL3G4200D(OUT_TEMP);
       printf("the temperature is :%d\n", temperature);
       
       X_Y_Z_display();
       memset(x_y_z, 0, 6);
       
       usleep(400000);
    }
    
    /*4.�ͷ���Դ,�ر��豸*/
    free(gy50_data.msgs);
    close(fd);  
    exit(0);	
}
   
/*��l3gд��һ���ֽ�*/
void Single_WriteL3G4200D(unsigned char REG_Address,unsigned char REG_data)
{
    /*1. ������Ϣ*/
    gy50_data.nmsgs = 1;   //д����
    gy50_data.msgs[0].len = 2;  //д�����ݵ��ֽڳ���,
    gy50_data.msgs[0].flags = 0;  //д����
    gy50_data.msgs[0].addr = SlaveAddress; //�豸��ַ 
    gy50_data.msgs[0].buf = (unsigned char *)malloc(2);  //Ϊbuf����ռ�
    if(NULL == gy50_data.msgs[0].buf)
        printf("get the senfbuf fail!\n"); 
    gy50_data.msgs[0].buf[0] = REG_Address;  //д���ֽڵ�ַ
    gy50_data.msgs[0].buf[1] = REG_data;  //д�������
    
    /*2.l3g4200������Ϣ*/
    ioctl(fd, I2C_RDWR, (unsigned long)&gy50_data);    	
}

/*��l3g����һ���ֽ�*/
unsigned char Single_ReadL3G4200D(unsigned char REG_Address)
{      
    unsigned char read_data=0;                    
    /*1.д��ӻ���ַ�ͼĴ�����ַ*/   
    gy50_data.nmsgs = 2;   //��Ϣ����
    gy50_data.msgs[0].len = 1;  //�ֽڳ���,�������ӻ���ַ,��Ϊ��ʼ���Ѿ�д�ڴӻ���ַ
    gy50_data.msgs[0].flags = 0;  //д����
    gy50_data.msgs[0].addr = SlaveAddress; //�ӻ���ַ
    gy50_data.msgs[0].buf[0] = REG_Address;  //д���ֽڵ�ַ
      
    gy50_data.msgs[1].len = 1;  //�ֽڳ���,�������ӻ���ַ,��Ϊ��ʼ���Ѿ�д�ڴӻ���ַ
    gy50_data.msgs[1].flags = 1;  //������
    gy50_data.msgs[1].addr = SlaveAddress; //�ӻ���ַ
    gy50_data.msgs[1].buf = (unsigned char *)malloc(2);  //Ϊbuf����ռ�
    if(NULL ==  gy50_data.msgs[1].buf )
        printf("get the recievebuf fail!\n");
    gy50_data.msgs[1].buf[0] = 1;  //���յ������ݴ��
  
    /*2.��gy50_data�ж�������*/
    ioctl(fd, I2C_RDWR, (unsigned long)&gy50_data);       
    read_data =  gy50_data.msgs[1].buf[0]; 
    free(gy50_data.msgs[1].buf);
    
    return read_data; 
}

unsigned char X_Y_Z_display(void)
{  
    float temp=0;
    short dis_data=0;
    
    /*1. ��ȡx��ĽǼ��ٶ�*/
    x_y_z[0] = Single_ReadL3G4200D(OUT_X_L);     
    x_y_z[1] = Single_ReadL3G4200D(OUT_X_H);
 
    dis_data=(short)((x_y_z[1]<<8) + x_y_z[0]);       //�ϳ�����   
    
    temp=(float)dis_data*(0.00875);         // �����ֲ� ��9ҳ��2000��/������
    dis_data=(short)temp;
        printf("the X: %d\n", dis_data); 
        
    dis_data=0;
    temp =0;
    
    /*2. ��ȡy��ĽǼ��ٶ�*/
    x_y_z[2] = Single_ReadL3G4200D(OUT_Y_L);     
    x_y_z[3] = Single_ReadL3G4200D(OUT_Y_H);
   
    dis_data=(short)((x_y_z[3]<<8) + x_y_z[2]);       //�ϳ�����   
    
    temp=(float)dis_data*(0.00875);         // �����ֲ� ��9ҳ��2000��/������
    dis_data=(short)temp;
        printf("the Y: %d\n", dis_data); 
        
    dis_data=0;
    temp =0;
 
    /*3. ��ȡz��ĽǼ��ٶ�*/
    x_y_z[4] = Single_ReadL3G4200D(OUT_Z_L);     
    x_y_z[5] = Single_ReadL3G4200D(OUT_Z_H);
  
    dis_data=(short)((x_y_z[5]<<8) + x_y_z[4]);       //�ϳ�����   
    
    temp=(float)dis_data*(0.00875);         // �����ֲ� ��9ҳ��2000��/������
    dis_data=(short)temp;
        printf("the Z: %d\n", dis_data); 
        
    dis_data=0;
    temp =0;
    
    return 0;	
}