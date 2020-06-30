#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>

#define  PORT  4545
#define  BUFF_SIZE 128
int main(int argc, char *argv[])
{ 
  	
    int sockfd;
    char buffer[BUFF_SIZE];
    struct sockaddr_in sockaddr_cilent;
    struct sockaddr_in sockaddr_server;
   
   if(argc != 2)
   {
      printf("usage : %s server_ip\n", argv[0]);	
   }
   /*创建套接字*/
    sockfd = socket(AF_INET, SOCK_DGRAM, 0); 
    if(sockfd < 0)
    {
     printf("sockfd fail!\n");	
    }
    
    
   /*发送数据*/
   
   bzero(&sockaddr_server, sizeof( struct sockaddr_in));  //清空结构体地址
   sockaddr_server.sin_port = htons(PORT);
   sockaddr_server.sin_family = AF_INET;
   sockaddr_server.sin_addr.s_addr = inet_addr(argv[1]);
  // sockaddr_server.sin_addr.s_addr = inet_("192.168.0.3");
   //inet_aton(argv[1], &sockaddr_server.sin_addr.s_addr);
   while(1)
   { 
   
     fgets(buffer, BUFF_SIZE, stdin);
     int n = sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)(&sockaddr_server),  sizeof(struct sockaddr));
     
     printf("%s\n", buffer);
     bzero(buffer,BUFF_SIZE);
   }
   
   /*关闭套接字*/
   close(sockfd);
   return 0;	
}