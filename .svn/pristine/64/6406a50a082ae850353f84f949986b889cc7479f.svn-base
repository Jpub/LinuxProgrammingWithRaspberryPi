#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define FIFOFILE    "fifo"

int main(int argc, char** argv)
{
   int n, fd;
   char buf[BUFSIZ];

   if((fd = open(FIFOFILE, O_WRONLY)) < 0) {    /* FIFO를 오픈한다. */
      perror("open( )");
      return -1;
   }

   while ((n = read(0, buf, sizeof(buf))) > 0)           /* 키보드로 부터 데이터를 입력 받는다 */ 
      write(fd, buf, n);                                               /* FIFO로 데이터를 보낸다. */

   close(fd);

   return 0;
}

