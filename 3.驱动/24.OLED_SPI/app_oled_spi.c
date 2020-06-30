/**************************************************
*�ļ���: app_oled_spi.c
*��������:2016-11-22
*�ļ�����: spi����oled��Ӧ�ó���. ֧�ֵ����ַ�,�ַ�������ʾ,����.֧��6*8�ĵ���, 8*16�ĵ���.         
*Ӳ��ƽ̨: MINI2440 
*��������:
* 1. GND ��Դ��
* 2. VCC ��Դ����3��5.5V��
* 3. D0 OLED �� D0 �ţ�SPI0��CLK����
* 4. D1 OLED �� D1 �ţ��� SPI0 MOSI����
* 5. RES OLED �� RES��������λ���͵�ƽ��λ��, GPF(5).
* 6. DC OLED �� D/C �ţ����ݺ�������ƹܽ�, GPF(3).
* 7. CS OLED �� CS#�ţ�Ҳ����Ƭѡ�ܽ�  ,  SPI0��nSS����.  
****************************************************/

#include  <stdlib.h>
#include  <stdio.h>
#include  <string.h>
#include  <sys/types.h>
#include  <sys/stat.h>
#include  <fcntl.h>
#include  <unistd.h>
#include  <sys/ioctl.h>
#include  "oledfont.h"
#include  "oled_spi_driver.h"

int main(void)
{
    int page = 0;
    int col = 0;
    int fd;
    unsigned char data=0xf;
    unsigned char chr = 'b';
    
    fd = open("/dev/oled", O_RDWR);
    if (fd < 0)
    {
        perror("can't open /dev/oled\n");
        return -1;
    }	
    /*��ʼ��������*/
    ioctl(fd, OLED_CMD_INIT);
    ioctl(fd, OLED_CMD_CLEAR_ALL);
    
    /*�趨����*/
    usleep(200);
    
    OLEDPrint_8x16(fd, 4, 64, "hello!");
    exit(0);	  
}



/*��ʾ�����ַ� , 8*16����*/ 
void OLEDPutChar_8x16(int fd, int page, int col, char c)
{
    int i = 0;
    unsigned char chr=0;
    
    const unsigned char *dots = F8X16; /*��ȡ�ֿ��ַ, 8*16�ĵ���,����Ϊ8,����Ϊ16*/
    
    chr = c - ' '; /*��ȡƫ��   " "��ascll��ֵΪ32,���ֿ�������ڵ�һλ*/
  
    ioctl(fd, OLED_CMD_SET_POS, page | (col << 8)); /*��������λ��*/
    
    write(fd, &dots[chr*16], 8); /*��ȡƫ��, ��д��8���ֽ�, Ҳ����8*8 = 64�����ص�*/

    ioctl(fd, OLED_CMD_SET_POS, (page+1) | ((col) << 8)); /*д�ַ����°��, ��ȡģ��ʽ����*/
    
    write(fd, &dots[chr*16+8], 8); 
}

/*��ʾ�����ַ� , 6*8����*/ 
void OLEDPutChar_6x8(int fd, int page, int col, char c)
{
    int i = 0;
    unsigned char chr=0;
    
    const unsigned char *dots = F6x8; /*��ȡ�ֿ��ַ, 6*8�ĵ���,����Ϊ6,����Ϊ8*/
    
    chr = c - ' '; /*��ȡƫ��   " "��ascll��ֵΪ32,���ֿ�������ڵ�һλ*/
  
    ioctl(fd, OLED_CMD_SET_POS, page | (col << 8)); /*��������λ��*/
    
    write(fd, &dots[chr*6], 6); /*��ȡƫ��, ��д��6���ֽ�, Ҳ����6*8 = 48�����ص�*/
}


/*��ʾ�ַ���8x16*/
void OLEDPrint_8x16(int fd, int page, int col, char *str)
{
    int i = 0;
    
    ioctl(fd, OLED_CMD_CLEAR_PAGE, page); /*����ҳ*/
    ioctl(fd, OLED_CMD_CLEAR_PAGE, page+1);
    while (str[i])
    {
        OLEDPutChar_8x16(fd, page, col, str[i]);
        col += 8;
        if (col > 127)
        {
            col = 0;
            page += 2;
            ioctl(fd, OLED_CMD_CLEAR_PAGE, page);
            ioctl(fd, OLED_CMD_CLEAR_PAGE, page+1);
        }
        i++;
    }
}

/*��ʾ�ַ���6x8*/
void OLEDPrint_6x8(int fd, int page, int col, char *str)
{
    int i = 0;
    
    ioctl(fd, OLED_CMD_CLEAR_PAGE, page); /*����ҳ*/
    while (str[i])
    {
        OLEDPutChar_6x8(fd, page, col, str[i]);
        col += 8;
        if (col > 127)
        {
            col = 0;
            page += 1;
            ioctl(fd, OLED_CMD_CLEAR_PAGE, page);
          
        }
        i++;
    }
}

void write_line(int fd)  /*��ֱ��,����Ϊ1ҳ�ĳ��ȵ�ֱ��*/
{  
     unsigned int i,j;
     unsigned int data=255;
    
     ioctl(fd, OLED_CMD_SET_POS, 4);
     for(j =0; j<128; j++)
     {
         write(fd, &data, 1);
     }		     
}