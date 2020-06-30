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
   
   /*����socket�׽���*/
   sktfd = socket(AF_INET, SOCK_STREAM, 0);
   
   /*�󶨵�ַ*/
   bzero(&sockaddr_server, sizeof(struct sockaddr_in));
   sockaddr_server.sin_port = htons(1234);
   sockaddr_server.sin_family = AF_INET;
   sockaddr_server.sin_addr.s_addr = htonl(INADDR_ANY);
   bind(sktfd, (struct sockaddr*)&sockaddr_server, sizeof(struct sockaddr));
   
   /*�����˿�*/
   listen(sktfd, 5);
   
    	
   /*�ȴ�����������*/
   int size_cilent = sizeof(sizeof(struct sockaddr));
   int acpfd =  accept(sktfd, (struct sockaddr*)&sockaddr_client, &size_cilent);
   
   int ret = 0;
   while(1)
   {
	   /*�շ�����*/
	   ret = recv(acpfd, buffer, 100, 0);
	   while((-1) == ret);
	  	 printf("tcpserver recv fail!\n");  	 
	   buffer[ret] = '\0';  //�ַ�����β�ӽ�����
	   printf("%s\n", buffer);   
	   
	   sleep(1); 
   }
   /*�ر������׽���*/
    close(acpfd);
    close(sktfd);    
    return 0;
} 

