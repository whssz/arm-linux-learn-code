#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/types.h>
#include <sys/ioctl.h>
#include  <stdlib.h>



#define   SlaveAddress  0x50
#define   I2C_RDWR   0x0707
void Single_WriteL3G4200D(unsigned char REG_Address,unsigned char REG_data);
unsigned char Single_ReadL3G4200D(unsigned char REG_Address);
struct i2c_msg {
	unsigned short addr;	/* slave address			*/
	unsigned short flags;   /*��д����,0д1��*/
	unsigned short len;		/* msg length				*/
	unsigned char *buf;		/* pointer to msg data			*/
};

 
struct i2c_rdwr_ioctl_data {
	struct i2c_msg  *msgs;	/* pointers to i2c_msgs */
	unsigned int nmsgs;	/* number of i2c_msgs */
};

int fd;
   struct i2c_rdwr_ioctl_data e2prom_data;
int main(void)
{
    
    
    //1.�������豸
    fd = open("/dev/i2c-0", O_RDWR);
    if(fd < 0)
       printf("open the i2c_bus fail!\n");
       
    e2prom_data.msgs = (struct i2c_msg *)malloc(2*sizeof(struct i2c_msg));
    if(NULL ==  e2prom_data.msgs)
        printf("get the msgsbuf fail!\n");
   
    //2.����д��eeprom����Ϣ
   /* e2prom_data.nmsgs = 1;   //��Ϣ����
    e2prom_data.msgs[0].len = 2;  //�ֽڳ���,2���ֽ�,д���ַ������
    e2prom_data.msgs[0].flags = 0;  //д����
    e2prom_data.msgs[0].addr = 0x50; //�豸��ַ
    e2prom_data.msgs[0].buf = (unsigned char *)malloc(2);  //Ϊbuf����ռ�
    if(NULL == e2prom_data.msgs[0].buf)
        printf("get the senfbuf fail!\n");
    e2prom_data.msgs[0].buf[0] = 0x10;  //д���ֽڵ�ַ
    e2prom_data.msgs[0].buf[1] = 0x30;  //д�������
    
    //3.��eeprom������Ϣ
    ioctl(fd, I2C_RDWR, (unsigned long)&e2prom_data);  
   */  
    Single_WriteL3G4200D(0x10, 0x30);
    Single_WriteL3G4200D(0x20, 0x60);
    Single_WriteL3G4200D(0x21, 0x61);
    Single_WriteL3G4200D(0x22, 0x62);
    Single_WriteL3G4200D(0x23, 0x63);

    //4.������eeprom��ȡ���ݵ���Ϣ
    e2prom_data.nmsgs = 2;   //��Ϣ����
    e2prom_data.msgs[0].len = 1;  //�ֽڳ���,�������ӻ���ַ,��Ϊ��ʼ���Ѿ�д�ڴӻ���ַ
    e2prom_data.msgs[0].flags = 0;  //д����
    e2prom_data.msgs[0].addr = 0x50; //�ӻ���ַ
    e2prom_data.msgs[0].buf[0] = 0x10;  //д���ֽڵ�ַ
      
    e2prom_data.msgs[1].len = 1;  //�ֽڳ���,�������ӻ���ַ,��Ϊ��ʼ���Ѿ�д�ڴӻ���ַ
    e2prom_data.msgs[1].flags = 1;  //������
    e2prom_data.msgs[1].addr = 0x50; //�ӻ���ַ
    e2prom_data.msgs[1].buf = (unsigned char *)malloc(2);  //Ϊbuf����ռ�
     if(NULL ==  e2prom_data.msgs[1].buf )
        printf("get the recievebuf fail!\n");
    e2prom_data.msgs[1].buf[0] = 0;  //���յ������ݴ��
  
    //5.��eeprom�ж�������,unsigned longע�������unsigned long�� ��ַ
    ioctl(fd, I2C_RDWR, (unsigned long)&e2prom_data); 
    
    printf("the data ls 0x%x\n",  e2prom_data.msgs[1].buf[0]);
    
    //////////////////////////////////////////////////////////
    
    e2prom_data.nmsgs = 2;   //��Ϣ����
    e2prom_data.msgs[0].len = 1;  //�ֽڳ���,�������ӻ���ַ,��Ϊ��ʼ���Ѿ�д�ڴӻ���ַ
    e2prom_data.msgs[0].flags = 0;  //д����
    e2prom_data.msgs[0].addr = 0x50; //�ӻ���ַ
    e2prom_data.msgs[0].buf[0] = 0x20;  //д���ֽڵ�ַ
      
    e2prom_data.msgs[1].len = 1;  //�ֽڳ���,�������ӻ���ַ,��Ϊ��ʼ���Ѿ�д�ڴӻ���ַ
    e2prom_data.msgs[1].flags = 1;  //������
    e2prom_data.msgs[1].addr = 0x50; //�ӻ���ַ
    e2prom_data.msgs[1].buf = (unsigned char *)malloc(2);  //Ϊbuf����ռ�
     if(NULL ==  e2prom_data.msgs[1].buf )
        printf("get the recievebuf fail!\n");
    e2prom_data.msgs[1].buf[0] = 0;  //���յ������ݴ��
  
    //5.��eeprom�ж�������,unsigned longע�������unsigned long�� ��ַ
    ioctl(fd, I2C_RDWR, (unsigned long)&e2prom_data); 
    
    printf("the data ls 0x%x\n",  e2prom_data.msgs[1].buf[0]);
    
    e2prom_data.nmsgs = 2;   //��Ϣ����
    e2prom_data.msgs[0].len = 1;  //�ֽڳ���,�������ӻ���ַ,��Ϊ��ʼ���Ѿ�д�ڴӻ���ַ
    e2prom_data.msgs[0].flags = 0;  //д����
    e2prom_data.msgs[0].addr = 0x50; //�ӻ���ַ
    e2prom_data.msgs[0].buf[0] = 0x21;  //д���ֽڵ�ַ
      
    e2prom_data.msgs[1].len = 1;  //�ֽڳ���,�������ӻ���ַ,��Ϊ��ʼ���Ѿ�д�ڴӻ���ַ
    e2prom_data.msgs[1].flags = 1;  //������
    e2prom_data.msgs[1].addr = 0x50; //�ӻ���ַ
    e2prom_data.msgs[1].buf = (unsigned char *)malloc(2);  //Ϊbuf����ռ�
     if(NULL ==  e2prom_data.msgs[1].buf )
        printf("get the recievebuf fail!\n");
    e2prom_data.msgs[1].buf[0] = 0;  //���յ������ݴ��
  
    //5.��eeprom�ж�������,unsigned longע�������unsigned long�� ��ַ
    ioctl(fd, I2C_RDWR, (unsigned long)&e2prom_data); 
    
    printf("the data ls 0x%x\n",  e2prom_data.msgs[1].buf[0]);
    
    e2prom_data.nmsgs = 2;   //��Ϣ����
    e2prom_data.msgs[0].len = 1;  //�ֽڳ���,�������ӻ���ַ,��Ϊ��ʼ���Ѿ�д�ڴӻ���ַ
    e2prom_data.msgs[0].flags = 0;  //д����
    e2prom_data.msgs[0].addr = 0x50; //�ӻ���ַ
    e2prom_data.msgs[0].buf[0] = 0x22;  //д���ֽڵ�ַ
      
    e2prom_data.msgs[1].len = 1;  //�ֽڳ���,�������ӻ���ַ,��Ϊ��ʼ���Ѿ�д�ڴӻ���ַ
    e2prom_data.msgs[1].flags = 1;  //������
    e2prom_data.msgs[1].addr = 0x50; //�ӻ���ַ
    e2prom_data.msgs[1].buf = (unsigned char *)malloc(2);  //Ϊbuf����ռ�
     if(NULL ==  e2prom_data.msgs[1].buf )
        printf("get the recievebuf fail!\n");
    e2prom_data.msgs[1].buf[0] = 0;  //���յ������ݴ��
  
    //5.��eeprom�ж�������,unsigned longע�������unsigned long�� ��ַ
    ioctl(fd, I2C_RDWR, (unsigned long)&e2prom_data); 
    
    printf("the data ls 0x%x\n",  e2prom_data.msgs[1].buf[0]);
    
    
   e2prom_data.nmsgs = 2;   //��Ϣ����
    e2prom_data.msgs[0].len = 1;  //�ֽڳ���,�������ӻ���ַ,��Ϊ��ʼ���Ѿ�д�ڴӻ���ַ
    e2prom_data.msgs[0].flags = 0;  //д����
    e2prom_data.msgs[0].addr = 0x50; //�ӻ���ַ
    e2prom_data.msgs[0].buf[0] = 0x23;  //д���ֽڵ�ַ
      
    e2prom_data.msgs[1].len = 1;  //�ֽڳ���,�������ӻ���ַ,��Ϊ��ʼ���Ѿ�д�ڴӻ���ַ
    e2prom_data.msgs[1].flags = 1;  //������
    e2prom_data.msgs[1].addr = 0x50; //�ӻ���ַ
    e2prom_data.msgs[1].buf = (unsigned char *)malloc(2);  //Ϊbuf����ռ�
     if(NULL ==  e2prom_data.msgs[1].buf )
        printf("get the recievebuf fail!\n");
    e2prom_data.msgs[1].buf[0] = 0;  //���յ������ݴ��
  
    //5.��eeprom�ж�������,unsigned longע�������unsigned long�� ��ַ
    ioctl(fd, I2C_RDWR, (unsigned long)&e2prom_data); 
    
    printf("the data ls 0x%x\n",  e2prom_data.msgs[1].buf[0]);
  
    //6.�ر��豸
    close(fd);
    
    return 0;	
}


/*��l3gд��һ���ֽ�*/
void Single_WriteL3G4200D(unsigned char REG_Address,unsigned char REG_data)
{
    /*1. ������Ϣ*/
    e2prom_data.nmsgs = 1;   //д����
    e2prom_data.msgs[0].len = 2;  //д�����ݵ��ֽڳ���,
    e2prom_data.msgs[0].flags = 0;  //д����
    e2prom_data.msgs[0].addr = SlaveAddress; //�豸��ַ 
    e2prom_data.msgs[0].buf = (unsigned char *)malloc(2);  //Ϊbuf����ռ�
    if(NULL == e2prom_data.msgs[0].buf)
        printf("get the senfbuf fail!\n"); 
    e2prom_data.msgs[0].buf[0] = REG_Address;  //д���ֽڵ�ַ
    e2prom_data.msgs[0].buf[1] = REG_data;  //д�������
    
    /*2.l3g4200������Ϣ*/
    ioctl(fd, I2C_RDWR, (unsigned long)&e2prom_data);    	
}

/*��l3g����һ���ֽ�*/
unsigned char Single_ReadL3G4200D(unsigned char REG_Address)
{                         
    /*1.д��ӻ���ַ�ͼĴ�����ַ*/   
    e2prom_data.nmsgs = 2;   //��Ϣ����
    e2prom_data.msgs[0].len = 1;  //�ֽڳ���,�������ӻ���ַ,��Ϊ��ʼ���Ѿ�д�ڴӻ���ַ
    e2prom_data.msgs[0].flags = 0;  //д����
    e2prom_data.msgs[0].addr = SlaveAddress; //�ӻ���ַ
    e2prom_data.msgs[0].buf = (unsigned char *)malloc(1);  //Ϊbuf����ռ�
    if(NULL == e2prom_data.msgs[0].buf)
         printf("get the senfbuf fail!\n");
    e2prom_data.msgs[0].buf[0] = REG_Address;  //д���ֽڵ�ַ
      
    e2prom_data.msgs[1].len = 1;  //�ֽڳ���,�������ӻ���ַ,��Ϊ��ʼ���Ѿ�д�ڴӻ���ַ
    e2prom_data.msgs[1].flags = 1;  //������
    e2prom_data.msgs[1].addr = SlaveAddress; //�ӻ���ַ
    e2prom_data.msgs[1].buf = (unsigned char *)malloc(2);  //Ϊbuf����ռ�
    if(NULL ==  e2prom_data.msgs[1].buf )
        printf("get the recievebuf fail!\n");
    e2prom_data.msgs[1].buf[0] = 1;  //���յ������ݴ��
  
    /*2.��gy50_data�ж�������*/
    ioctl(fd, I2C_RDWR, (unsigned long)&e2prom_data); 
    
    printf("the data ls %d\n",  e2prom_data.msgs[1].buf[0]);        
  
    return e2prom_data.msgs[1].buf[0]; 
}