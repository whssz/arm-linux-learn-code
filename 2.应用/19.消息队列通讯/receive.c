#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define MSGDATA_SIZE 1024
int msgid = 0;

struct mymsg{
   long type;
   char msd_data[MSGDATA_SIZE];
};
void child_process()
{  
    struct mymsg msg_buf;
    
    while(1)
    {
     /*接收信息*/
     msgrcv(msgid, &msg_buf, sizeof(struct mymsg), 0, 0);  //type为0, 直接选取消息队列第一条消息
     /*打印信息*/
     printf("%s\n", msg_buf.msd_data);
    }
    
   return;
}

int main(void)
{  
    int msgid;
    int chfd;
    int i;
   /*获取消息队列*/
    msgid = msgget(1024, IPC_EXCL);
       
   /*创建3个子进程*/
   for(i=0; i<3; i++)
   {
      chfd = fork();
     
      if(chfd < 0)
          printf("creat child process error!\n");
      else if(0 == chfd)
          child_process();
   }
        	   	 	
   return 0;	
}

