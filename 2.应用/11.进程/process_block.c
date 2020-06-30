#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ipc.h>

int main(void)
{ 
    pthread_t tid;
    pthread_creat(&tid, NULL, func, NULL); 
    
       
    exit(0);
}
