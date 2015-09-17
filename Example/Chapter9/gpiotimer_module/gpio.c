#include <stdio.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char** argv)
{
    char buf[BUFSIZ];
    char i = 0;
    int fd = -1;
    memset(buf, 0, BUFSIZ);

    printf("GPIO Set : %s\n", argv[1]);
    fd = open("/dev/gpioled", O_RDWR);                /* GPIO 장치 파일을 오픈한다. */

    write(fd, argv[1], strlen(argv[1]), NULL);            /* 커널 모듈에 데이터를 쓴다. */

    read(fd, buf, strlen(argv[1]), NULL);                  /* 커널로 부터 데이터를 읽는다. */
    
    printf("Read data : %s\n", buf);                     /* 읽은 데이터를 화면에 출력한다. */

    close(fd);                                           /* 사용이 끝난 장치 파일을 닫는다. */

    return 0;
}

