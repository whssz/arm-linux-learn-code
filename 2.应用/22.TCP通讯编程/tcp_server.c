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
   char buffer[100];
   
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
   
    	
   /*等待服务器连接*/
   int size_cilent = sizeof(sizeof(struct sockaddr));
   int acpfd =  accept(sktfd, (struct sockaddr*)&sockaddr_client, &size_cilent);
   
   int ret = 0;
   while(1)
   {
	   /*收发数据*/
	   ret = recv(acpfd, buffer, 100, 0);
	   while((-1) == ret);
	  	 printf("tcpserver recv fail!\n");  	 
	   buffer[ret] = '\0';  //字符串结尾加结束符
	   printf("%s\n", buffer);   
	   
	   sleep(1); 
   }
   /*关闭连接套接字*/
    close(acpfd);
    close(sktfd);    
    return 0;
} 

