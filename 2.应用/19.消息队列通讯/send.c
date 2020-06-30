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
 
  /*������Ϣ����*/
  msgid = msgget(1024, IPC_CREAT);
  
  /*�ȴ�������������*/
  while(1)
  {
     /*��ȡ��Ϣ����*/
     printf("please input msg_type!\n");
     scanf("%d", &msg_type);
     /*����û������������Ϣ����Ϊ0, �˳���ѭ��*/
     if(0 == msg_type)
     break;
     /*��ȡ��Ϣ����*/
     printf("please input message content\n!");
     scanf("%s", str);	
     
     msgbuf.type = msg_type;
     strcpy(msgbuf.msd_datap, str);
     msgsnd(msgid, &msgbuf, sizeof(struct mymsg), 0);
  }
 
  
  /*ɾ����Ϣ����*/	
  msgctl(msgid, IPC_RMID, 0);
  
  return 0;	
}