#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>

#define  PORT  4545
#define  BUFF_SIZE 128

int main(void)
{ 
   int sockfd;
   struct sockaddr_in sockaddr_server;
   struct sockaddr_in sockaddr_cilent;
   char buffer[BUFF_SIZE];
   int addr_lenth;
	
  /*����socket�׽���*/
   sockfd = socket(AF_INET, SOCK_DGRAM, 0);
   if(sockfd < 0)
   {
     printf("sockfd fail!\n");	
   }

  /*�󶨵�ַ*/
   bzero(&sockaddr_server, sizeof( struct sockaddr_in));  //��սṹ���ַ
   sockaddr_server.sin_port = htons(PORT);
   sockaddr_server.sin_family = AF_INET;
   sockaddr_server.sin_addr.s_addr = htonl(INADDR_ANY);
   
   bind(sockfd, (struct sockaddr*)&sockaddr_server, sizeof(struct sockaddr));
  
  /*��������*/
   while(1)
   { 
    
     addr_lenth =  sizeof(struct sockaddr_in);
     bzero(buffer, sizeof(buffer));
    
     int n = recvfrom(sockfd, buffer, BUFF_SIZE, 0, (struct sockaddr *)(&sockaddr_cilent), &addr_lenth); 
      	
     buffer[n] = 0;
     printf("%d\n",n);
     printf("zzz\n");
     printf("server revieve : %s\n", buffer);
     
   }
  /*��������*/
   
 
  /*�ر��׽���*/	 
  close(sockfd);
	
  return 0;	
}