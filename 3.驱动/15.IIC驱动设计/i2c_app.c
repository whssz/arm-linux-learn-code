#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>


int main(void)
{
    int fd;
    char write_data[256];
    char read_data[256];
    int i;
    
    fd = open("/sys/bus/i2c/devices/0-0050/eeprom", O_RDWR);
    
    for(i=0; i<256; i++)
        write_data[i] = i;
       
     lseek(fd, 0 ,SEEK_SET);
     write(fd, write_data, 256);
     
     lseek(fd, 0, SEEK_SET);
     read(fd, read_data, 256);
     
    for(i=0; i<256; i++)
    {   
    	if(0 == i%16)
    	    printf("\n");
        printf("%3d", read_data[i]);	
    } 
     
     printf("\n");
     
     close(fd);
     
   
     	
}