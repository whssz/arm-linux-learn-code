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
  struct sembuf sops;  /*操作结构体*/
  
  fd = open("./board.txt", O_RDWR | O_APPEND);
  
  /*获取键值*/
  key = ftok("/home", 1);  /*项目编号位1*/
  
  /*创建信号量*/
  get = semget(key, 1, IPC_CREAT);  /*信号量个数位1*/
  
  /*查看,并设置信号量初值*/
  semctl(get, 0, SETVAL, 1); /*操作的第一个信号量， 初始设置位1*/
  
  /*操作信号量*/
  sops.sem_num = 0;
  sops.sem_op = -1;
  sops.sem_flg = SEM_UNDO;
  semop(get, &sops, 1);
  
  write(fd, "wu", 2);
  /*进程睡眠10s*/
  sleep(10);
  
  write(fd, "hao", 3);
  
  /*释放信号量*/
  sops.sem_num = 0;
  sops.sem_op = 1;
  sops.sem_flg = SEM_UNDO;
  semop(get, &sops, 1);
  
  close(fd);
	
  return 0;	
}
