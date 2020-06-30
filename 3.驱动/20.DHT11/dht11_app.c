#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main(void)
{
    int fd;
    unsigned char envir[2];
    unsigned int i=10;
    int ret;
    fd = open("dev/dht_mscdrv", O_RDONLY);
    if(fd < 0)
    {
        printf("open dht_mscdev fail!\n");
        return 0;
    } 
      
    while(1)
    {  
        ret = read(fd, envir, 2); 
        if(ret < 0)
        {
            printf("get the envir fail!\n");
            sleep(3); 
            continue;           	
        } 
        printf("the humi: %d, the temp: %d\n\r", envir[0], envir[1]); 
        sleep(3);        	
    } 
    
    close(fd);  
    return 0;
}
