#include <unistd.h>
#include <sys/types.h>

int main(void)
{
   pid_t pid;
   pid = vfork();
   
   if(0 == pid)
   {   
       int ret;
       ret = execl("/bin/hello", "hello", NULL);
       if(ret < 0)
           perror("run fail!\n");
        exit(0);
   }
   else
   {
       printf("yes!\n");
       exit(0);
   }
}
