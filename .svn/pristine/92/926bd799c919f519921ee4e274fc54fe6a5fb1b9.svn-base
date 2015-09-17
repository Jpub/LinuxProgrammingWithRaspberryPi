#include <stdio.h>
#include <fcntl.h>
#include <string.h>

int ledControl(int gpio)
{
   int fd;
   char buf[BUFSIZ];

   fd = open("/sys/class/gpio/export", O_WRONLY);          /* 해당 GPIO 장치 사용 준비 */
   sprintf(buf, "%d", gpio);
   write(fd, buf, strlen(buf));
   close(fd);

   sprintf(buf, "/sys/class/gpio/gpio%d/direction", gpio);      /* 해당 GPIO 장치의 방향 설정 */
   fd = open(buf, O_WRONLY);
   /* 장치를 LED 출력을 위해서 설정 : 입력의 경우 write(fd, "in", 2); 사용 */
   write(fd, "out", 4);                                                          
   close(fd);

   sprintf(buf, "/sys/class/gpio/gpio%d/value", gpio);          /* 장치에 값 출력 */
   fd = open(buf, O_WRONLY);
   write(fd, "1", 2);
   close(fd);

   fd = open("/sys/class/gpio/unexport", O_WRONLY);     /* 사용한 GPIO 장치 해제하기 */
   sprintf(buf, "%d", gpio);
   write(fd, buf, strlen(buf));
   close(fd);

   return 0;
}

int main(int argc, char** argv)
{
   int gno;

   if(argc < 2) {
      printf("Usage : %s GPIO_NO\n", argv[0]);
      return -1;
   }
   gno = atoi(argv[1]);

   ledControl(gno);

   return 0;
}

