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
 
  while(1)
  { 	
     /*�ȴ�����������*/
      int size_cilent = sizeof(sizeof(struct sockaddr));
      int acpfd =  accept(sktfd, (struct sockaddr*)&sockaddr_client, &size_cilent);
      
      
    //  printf("the cilent IP is %s\n", inet_ntoa(ntohl(sockaddr_client.sin_addr.s_addr)));
      /*���̸߳����շ�����,���ڽ������ر�����*/
      int retfk = fork();
      if(0 == retfk)
      {
         /*�շ�����*/  
         while(1)
         {
          int ret = recv(acpfd, buffer, 200, 0);
          while((-1) == ret);
          printf("the recv counts are %d\n", ret);
          buffer[ret] = '\0';  //�ַ�����β�ӽ�����
          printf("the recv data is %s\n", buffer); 
          /*������յ������ݴ���200*/
          if(ret > 200)
          break;
        }
          /*�ر������׽���*/
          close(acpfd);
      }   
  }
   /*�ر������׽���*/
   close(sktfd);
     
   return 0;
} 

