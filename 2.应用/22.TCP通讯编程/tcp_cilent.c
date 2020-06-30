#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>

int main(int argc, char *argv[])
{ 
  int sktfd; 
  struct sockaddr_in sockaddr_client;
  unsigned char buffer[200];
  /*创建socket套接字*/ 
  sktfd = socket(AF_INET, SOCK_STREAM, 0);
  
  /*连接服务器*/
  bzero(&sockaddr_client, sizeof(struct sockaddr_in));
  sockaddr_client.sin_port = htons(1234);
  sockaddr_client.sin_family = AF_INET;
  //sockaddr_client.sin_addr.s_addr = inet_addr("192.168.0.3");
  printf("%s\n", argv[1]);
  sockaddr_client.sin_addr.s_addr = inet_addr(argv[1]);
  connect(sktfd, (struct sockaddr*)&sockaddr_client, sizeof(struct sockaddr));
  while(1)
  {	
   /*收发数据*/
   fgets(buffer, 200, stdin);  //从终端接收数据
  
   int ret = send(sktfd, buffer, strlen(buffer), 0);
   printf("the send count is %d\n", ret);
  
  }
  /*关闭套接字*/
  close(sktfd);
  
  return 0;	
}
