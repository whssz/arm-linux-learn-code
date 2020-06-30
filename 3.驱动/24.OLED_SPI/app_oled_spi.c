/**************************************************
*文件名: app_oled_spi.c
*创建日期:2016-11-22
*文件描述: spi驱动oled的应用程序. 支持单个字符,字符串的显示,画线.支持6*8的点阵, 8*16的点阵.         
*硬件平台: MINI2440 
*引脚连接:
* 1. GND 电源地
* 2. VCC 电源正（3～5.5V）
* 3. D0 OLED 的 D0 脚，SPI0的CLK引脚
* 4. D1 OLED 的 D1 脚，在 SPI0 MOSI引脚
* 5. RES OLED 的 RES，用来复位（低电平复位）, GPF(5).
* 6. DC OLED 的 D/C 脚，数据和命令控制管脚, GPF(3).
* 7. CS OLED 的 CS#脚，也就是片选管脚  ,  SPI0的nSS引脚.  
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
    /*初始化并清屏*/
    ioctl(fd, OLED_CMD_INIT);
    ioctl(fd, OLED_CMD_CLEAR_ALL);
    
    /*设定坐标*/
    usleep(200);
    
    OLEDPrint_8x16(fd, 4, 64, "hello!");
    exit(0);	  
}



/*显示单个字符 , 8*16点阵*/ 
void OLEDPutChar_8x16(int fd, int page, int col, char c)
{
    int i = 0;
    unsigned char chr=0;
    
    const unsigned char *dots = F8X16; /*获取字库地址, 8*16的点阵,横轴为8,纵轴为16*/
    
    chr = c - ' '; /*获取偏移   " "的ascll码值为32,在字库里面放在第一位*/
  
    ioctl(fd, OLED_CMD_SET_POS, page | (col << 8)); /*设置坐标位置*/
    
    write(fd, &dots[chr*16], 8); /*获取偏移, 并写入8个字节, 也就是8*8 = 64个像素点*/

    ioctl(fd, OLED_CMD_SET_POS, (page+1) | ((col) << 8)); /*写字符的下半段, 由取模方式决定*/
    
    write(fd, &dots[chr*16+8], 8); 
}

/*显示单个字符 , 6*8点阵*/ 
void OLEDPutChar_6x8(int fd, int page, int col, char c)
{
    int i = 0;
    unsigned char chr=0;
    
    const unsigned char *dots = F6x8; /*获取字库地址, 6*8的点阵,横轴为6,纵轴为8*/
    
    chr = c - ' '; /*获取偏移   " "的ascll码值为32,在字库里面放在第一位*/
  
    ioctl(fd, OLED_CMD_SET_POS, page | (col << 8)); /*设置坐标位置*/
    
    write(fd, &dots[chr*6], 6); /*获取偏移, 并写入6个字节, 也就是6*8 = 48个像素点*/
}


/*显示字符串8x16*/
void OLEDPrint_8x16(int fd, int page, int col, char *str)
{
    int i = 0;
    
    ioctl(fd, OLED_CMD_CLEAR_PAGE, page); /*清屏页*/
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

/*显示字符串6x8*/
void OLEDPrint_6x8(int fd, int page, int col, char *str)
{
    int i = 0;
    
    ioctl(fd, OLED_CMD_CLEAR_PAGE, page); /*清屏页*/
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

void write_line(int fd)  /*画直线,纵轴为1页的长度的直线*/
{  
     unsigned int i,j;
     unsigned int data=255;
    
     ioctl(fd, OLED_CMD_SET_POS, 4);
     for(j =0; j<128; j++)
     {
         write(fd, &data, 1);
     }		     
}