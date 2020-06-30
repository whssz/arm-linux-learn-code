#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int main(void)
{ 
  int fd;
  key_t key;
  int get;
  int int_num;
  struct sembuf sops;  /*�����ṹ��*/
  
  fd = open("./board.txt", O_RDWR | O_APPEND);
  
  /*��ȡ��ֵ*/
  key = ftok("/home", 1);  /*��Ŀ���λ1*/
  
  /*�����ź���*/
  get = semget(key, 1, IPC_CREAT);  /*�ź�������λ1*/
  
  /*�鿴,�������ź�����ֵ*/
  semctl(get, 0, SETVAL, 1); /*�����ĵ�һ���ź����� ��ʼ����λ1*/
  
  /*�����ź���*/
  sops.sem_num = 0;
  sops.sem_op = -1;
  sops.sem_flg = SEM_UNDO;
  semop(get, &sops, 1);
  
  write(fd, "wu", 2);
  /*����˯��10s*/
  sleep(10);
  
  write(fd, "hao", 3);
  
  /*�ͷ��ź���*/
  sops.sem_num = 0;
  sops.sem_op = 1;
  sops.sem_flg = SEM_UNDO;
  semop(get, &sops, 1);
  
  close(fd);
	
  return 0;	
}
