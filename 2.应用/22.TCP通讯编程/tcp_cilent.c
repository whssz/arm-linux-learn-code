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
  /*����socket�׽���*/ 
  sktfd = socket(AF_INET, SOCK_STREAM, 0);
  
  /*���ӷ�����*/
  bzero(&sockaddr_client, sizeof(struct sockaddr_in));
  sockaddr_client.sin_port = htons(1234);
  sockaddr_client.sin_family = AF_INET;
  //sockaddr_client.sin_addr.s_addr = inet_addr("192.168.0.3");
  printf("%s\n", argv[1]);
  sockaddr_client.sin_addr.s_addr = inet_addr(argv[1]);
  connect(sktfd, (struct sockaddr*)&sockaddr_client, sizeof(struct sockaddr));
  while(1)
  {	
   /*�շ�����*/
   fgets(buffer, 200, stdin);  //���ն˽�������
  
   int ret = send(sktfd, buffer, strlen(buffer), 0);
   printf("the send count is %d\n", ret);
  
  }
  /*�ر��׽���*/
  close(sktfd);
  
  return 0;	
}
