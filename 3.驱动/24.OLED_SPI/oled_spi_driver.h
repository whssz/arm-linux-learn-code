#ifndef _OLED_SPI_DRIVER_
#define _OLED_SPI_DRIVER_

#define MEM_IOC_MAGIC 'O'   //¶¨Òå»ÃÊý
#define OLED_CMD_INIT          _IO(MEM_IOC_MAGIC, 0)
#define OLED_CMD_CLEAR_ALL     _IO(MEM_IOC_MAGIC, 1)
#define OLED_CMD_SET_POS       _IOW(MEM_IOC_MAGIC, 3 ,unsigned short) 
#define OLED_CMD_CLEAR_PAGE    _IOW(MEM_IOC_MAGIC, 4 ,unsigned char) 

void OLEDPrint_6x8(int , int , int , char *);
void OLEDPrint_8x16(int , int , int , char *);
void OLEDPutChar_8x16(int , int , int , char );
void OLEDPutChar_6x8(int , int , int , char );
void write_line();
#endif
