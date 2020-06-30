#define GPCCON    (*(volatile unsigned long *)0x56000020)
#define GPDCON    (*(volatile unsigned long *)0x56000030)
#define GPGCON    (*(volatile unsigned long *)0x56000060)
#define LCDCON1   (*(volatile unsigned long *)0x4D000000)
#define LCDCON2   (*(volatile unsigned long *)0x4D000004)
#define LCDCON3   (*(volatile unsigned long *)0x4D000008)
#define LCDCON4   (*(volatile unsigned long *)0x4D00000C)
#define LCDCON5   (*(volatile unsigned long *)0x4D000010)
#define LCDSADDR1 (*(volatile unsigned long *)0x4D000014)
#define LCDSADDR2 (*(volatile unsigned long *)0x4D000018)
#define LCDSADDR3 (*(volatile unsigned long *)0x4D00001C)
#define TPAL      (*(volatile unsigned long *)0x4D000050)

#define CLKVAL 4   //10 = 100/((CLKVAL+1)*2)
#define VSPW 9
#define VBPD 0
#define VFPD 0
#define HSPW 4
#define HBPD 100
#define HFPD 0
#define LINEVAL 319
#define HOZVAL 239
unsigned short LCDBUFFER[320][240];

typedef unsigned int U32;
typedef unsigned short U16;
typedef unsigned char U8;

void lcd_port_init()
{
    GPDCON = 0xaaaaaaaa;
    GPCCON = 0xaaaaaaaa;		
}

void lcd_control_init()
{	
    LCDCON1 = (CLKVAL<<8)|(0x3<<5)|(0xc<<1)|(0<<0);
    LCDCON2 = (VBPD<<24)|(LINEVAL<<14)|(VFPD<<6)|(VSPW);
    LCDCON3 = (HBPD<<19)|(HOZVAL<<8)|(HFPD);
    LCDCON4 = (HSPW);
    LCDCON5 = (1<<11)|(1<<9)|(1<<8);
    
    LCDSADDR1 = (((U32)LCDBUFFER>>22)<<21) | (((U32)LCDBUFFER>>1)&0x1fffff);
    LCDSADDR2 = (((U32)LCDBUFFER+320*240*2)>>1)&0x1fffff;
    LCDSADDR3 = (0<<11) | (240*2/2);
    
    TPAL = 0;
}


void lcd_init()
{
    lcd_port_init();
    lcd_control_init();
    
    //打开LCD电源
    GPGCON |= (0x3<<8);
    LCDCON5 |= (1<<3);   
    LCDCON1 |= 1;
   
}

void point(unsigned int x,unsigned int y,unsigned int color)  //画点函数
{
    unsigned int red, green, blue;  //颜色的三原色
    
    red = (color>>19) & 0x1f;
    green = (color>>10) & 0x3f;
    blue = (color>>3) & 0x1f;
    LCDBUFFER[y][x] = (unsigned short)((red<<11)|(green<<5)|blue);//5 6 5 红 绿 蓝，从rgb格式提取成5 6 5 的格式。
}

void Paint_Bmp(U16 x0,U16 y0,U16 wide,U16 high,const U8 *bmp)
{
    U16 x,y;
    U16 c;
    U32 p = 0;
    
    for( y = y0 ; y < y0+high ; y++ )
    {
    	for( x = x0 ; x < x0+wide ; x++ )
    	{
    	    c = bmp[p] | (bmp[p+1]<<8) ;//两个char 结合成一个short									
	    
	    if ( ( x < 240) && ( y < 320) )
	        LCDBUFFER[y][x] = c ; //存下当前像素点的16bit灰度
		
	    p = p + 2 ;	//指针加2
    	}
    }
}

void clearSrc(unsigned int color)
{
    TPAL = (1<<24)|(color&0xffffff); //保存rgb格式，24位	
}

extern unsigned char bmp[76800]; //定义数组

void lcd_test()
{
   //int x;
    
    //for(x=0;x<240;x++)
      //  point(x++,100,0xff0000);  //红色 
        
    Paint_Bmp(0,0,220,220,bmp); 
    
   // clearSrc(0xff0000);  //清屏为红色
}



//http://www.114la.com/other/rgb.htm


