#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>


#define MSGDATA_SIZE 1024

struct mymsg{
   long type;
   char msd_datap[MSGDATA_SIZE];
};


int main(void)
{ 
  int msgid;
  struct mymsg msgbuf;
  int msg_type;
  char str[1024];
 
  /*创建消息队列*/
  msgid = msgget(1024, IPC_CREAT);
  
  /*等待键盘输入数据*/
  while(1)
  {
     /*获取消息类型*/
     printf("please input msg_type!\n");
     scanf("%d", &msg_type);
     /*如果用户输入输入的消息类型为0, 退出该循环*/
     if(0 == msg_type)
     break;
     /*获取消息内容*/
     printf("please input message content\n!");
     scanf("%s", str);	
     
     msgbuf.type = msg_type;
     strcpy(msgbuf.msd_datap, str);
     msgsnd(msgid, &msgbuf, sizeof(struct mymsg), 0);
  }
 
  
  /*删除消息队列*/	
  msgctl(msgid, IPC_RMID, 0);
  
  return 0;	
}