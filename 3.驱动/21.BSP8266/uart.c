#include     <stdio.h>
#include     <stdlib.h> 
#include     <unistd.h>  
#include     <sys/types.h>
#include     <sys/stat.h>
#include     <fcntl.h> 
#include     <termios.h>
#include     <errno.h>
   
main()
{
    int fd;
    int i;
    int len;
    int n = 0;      
    char read_buf[256];
    char write_buf[256];
    char buff[]= "hello\r\n";
    struct termios opt; 
    struct termios oldopt; 
    
    //fd = open("/dev/ttyS0", O_RDWR | O_NOCTTY );    
    fd = open("/dev/s3c2410_serial2", O_RDWR | O_NOCTTY );
    if(fd == -1)
    {
        perror("open serial fail!\n");
        exit(0);
    }

    bzero( &opt, sizeof( opt ) );    
    if ( tcgetattr( fd,&opt) != 0)   
        {   
            perror("SetupSerial fail\n");  
            return -1;  
        }     

    opt.c_cflag |= (CLOCAL | CREAD);
    opt.c_cflag &= ~CSIZE;
    opt.c_cflag &= ~CRTSCTS;
    opt.c_iflag |= IGNPAR;
    opt.c_cflag |= CS8;
    opt.c_cflag &= ~CSTOPB;  
       
    opt.c_lflag = 0;
    //opt.c_oflag = 0;
/*
    opt.c_cflag &= ~PARENB; 
    opt.c_cflag &= ~INPCK;
*/

    cfsetispeed(&opt, B115200);
    cfsetospeed(&opt, B115200);
/*
    opt.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
 
    opt.c_oflag &= ~OPOST;
    opt.c_oflag &= ~(ONLCR | OCRNL);   
 
    opt.c_iflag &= ~(ICRNL | INLCR);
    opt.c_iflag &= ~(IXON | IXOFF | IXANY);    
*/   
    opt.c_cc[VTIME] = 10;
    opt.c_cc[VMIN] = 0;
    
    tcflush(fd, TCIOFLUSH);
 
    printf("configure complete\n");
    
    if(tcsetattr(fd, TCSANOW, &opt) != 0)
    {
        perror("serial error");
        return -1;
    }
    printf("start send and receive data\n");
  
    while(1)
    {    
        n = 0;
        len = 0;
        bzero(read_buf,  sizeof(read_buf));   
        bzero(write_buf, sizeof(write_buf));
        write(fd,buff,16);
		
        while( (n = read(fd, read_buf, sizeof(read_buf))) > 0 )
        {
            for(i = len; i < (len + n); i++)
            {
                write_buf[i] = read_buf[i - len];
            }
            len += n;
        }
        write_buf[len] = '\0';
              
        printf("Len %d \n", len);
        printf("%s \n", write_buf);
 
        n = write(fd, write_buf, len);
        printf("write %d chars\n",n);

        sleep(2);
    }
}

