#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int main(void)
{  
   int sem_id;
   key_t key;
   struct sembuf sops;
   
   /*打开信号量*/
   key = ftok("/home", 2);
   sem_id = semget(key, 1, IPC_CREAT);
   
   /*获取信号量*/
   sops.sem_num = 0;
   sops.sem_op = -1;
   sops.sem_flg =  SEM_UNDO;  /*在程序运行错误的时候，系统自动释放信号量*/
   semop(sem_id, &sops, 1);
   	
   system("cp product.txt ./product1.txt");
   
   exit(0);
}

