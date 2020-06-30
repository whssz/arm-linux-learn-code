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
   
   /*���ź���*/
   key = ftok("/home", 2);
   sem_id = semget(key, 1, IPC_CREAT);
   
   /*��ȡ�ź���*/
   sops.sem_num = 0;
   sops.sem_op = -1;
   sops.sem_flg =  SEM_UNDO;  /*�ڳ������д����ʱ��ϵͳ�Զ��ͷ��ź���*/
   semop(sem_id, &sops, 1);
   	
   system("cp product.txt ./product1.txt");
   
   exit(0);
}

