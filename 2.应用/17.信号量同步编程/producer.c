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
   
  /*打开/创建文件*/
  fd = open("./product.txt",O_RDWR|O_CREAT,0666);
  
  /*打开信号量*/
  key = ftok("/home", 2);
  sem_id = semget(key, 1, IPC_CREAT);
  /*设置初值为0,让消费者无法获取信号量*/
  semctl(sem_id, 0, SETVAL, 0);
  /*等待10s*/
  sleep(10);

  /*写入数据*/
  write(fd, "the product was completed!",30);
  
  /*释放信号量*/
  sops.sem_num = 0;
  sops.sem_op = 1;
  sops.sem_flg =  SEM_UNDO;
  semop(sem_id, &sops, 1);
  /*关闭文件*/
  close(fd);
  	
  return 0;	
}
