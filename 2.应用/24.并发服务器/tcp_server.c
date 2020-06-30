#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>

int main(void)
{  
   int sktfd;
   struct sockaddr_in sockaddr_server;
   struct sockaddr_in sockaddr_client;
   unsigned char buffer[200];
   
   /*创建socket套接字*/
   sktfd = socket(AF_INET, SOCK_STREAM, 0);
   
   /*绑定地址*/
   bzero(&sockaddr_server, sizeof(struct sockaddr_in));
   sockaddr_server.sin_port = htons(1234);
   sockaddr_server.sin_family = AF_INET;
   sockaddr_server.sin_addr.s_addr = htonl(INADDR_ANY);
   bind(sktfd, (struct sockaddr*)&sockaddr_server, sizeof(struct sockaddr));
   
   /*监听端口*/
   listen(sktfd, 5);
 
  while(1)
  { 	
     /*等待服务器连接*/
      int size_cilent = sizeof(sizeof(struct sockaddr));
      int acpfd =  accept(sktfd, (struct sockaddr*)&sockaddr_client, &size_cilent);
      
      
    //  printf("the cilent IP is %s\n", inet_ntoa(ntohl(sockaddr_client.sin_addr.s_addr)));
      /*子线程负责收发数据,并在结束处关闭链接*/
      int retfk = fork();
      if(0 == retfk)
      {
         /*收发数据*/  
         while(1)
         {
          int ret = recv(acpfd, buffer, 200, 0);
          while((-1) == ret);
          printf("the recv counts are %d\n", ret);
          buffer[ret] = '\0';  //字符串结尾加结束符
          printf("the recv data is %s\n", buffer); 
          /*如果接收到的数据大于200*/
          if(ret > 200)
          break;
        }
          /*关闭链接套接字*/
          close(acpfd);
      }   
  }
   /*关闭连接套接字*/
   close(sktfd);
     
   return 0;
} 

