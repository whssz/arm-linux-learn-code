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
  struct sembuf sops;
  int ret;
  
  fd = open("./board.txt", O_RDWR|O_APPEND);
   
  /*获取键值*/
  key = ftok("/home", 1);
  
  /*创建信号量*/
  get = semget(key, 1, IPC_CREAT);
  ret = semctl(get, 0, GETVAL);
  printf("the num is :%d\n", ret);
  
  /*获取信号量的状态*/
  sops.sem_num = 0;
  sops.sem_op = -1;
  sops.sem_flg = SEM_UNDO;
  semop(get, &sops, 1);
  
  write(fd, "gu liang jin",20);
  
  ret = semctl(get, 0, GETVAL);
  printf("the num is :%d\n", ret);
  /*释放信号量*/
  sops.sem_num = 0;
  sops.sem_op = 1;
  sops.sem_flg = SEM_UNDO;
  semop(get, &sops, 1);
  
  close(fd);
  	
  return 0;
}
