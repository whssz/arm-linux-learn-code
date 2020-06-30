#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int main(void)
{ 
   int fd;
   int sem_id;
   key_t key;
   struct sembuf sops;
   
  /*��/�����ļ�*/
  fd = open("./product.txt",O_RDWR|O_CREAT,0666);
  
  /*���ź���*/
  key = ftok("/home", 2);
  sem_id = semget(key, 1, IPC_CREAT);
  /*���ó�ֵΪ0,���������޷���ȡ�ź���*/
  semctl(sem_id, 0, SETVAL, 0);
  /*�ȴ�10s*/
  sleep(10);

  /*д������*/
  write(fd, "the product was completed!",30);
  
  /*�ͷ��ź���*/
  sops.sem_num = 0;
  sops.sem_op = 1;
  sops.sem_flg =  SEM_UNDO;
  semop(sem_id, &sops, 1);
  /*�ر��ļ�*/
  close(fd);
  	
  return 0;	
}
