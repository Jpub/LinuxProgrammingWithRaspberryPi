#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/types.h>

#define FBDEVICE "/dev/fb0"

typedef unsigned char ubyte;

unsigned short makepixel(unsigned char r, unsigned char g, unsigned char b) {
    return (unsigned short)(((r>>3)<<11)|((g>>2)<<5)|(b>>3));
}

static int DrawPoint(int fd, int x, int y, unsigned short color)
{
    struct fb_var_screeninfo vinfo;

    /* 현재 프레임버퍼에 대한 고정된 화면 정보를 얻어온다. */
    if(ioctl(fd, FBIOGET_VSCREENINFO, &vinfo) < 0) {
        perror("Error reading fixed information");
        return -1;
    }

    /* 색상 출력을 위한 위치를 구한다. */
    /* offset  = (X의_위치 + Y의_위치 x 해상도의_넓이) x 2  */
    int offset = (x + y*vinfo.xres)*2;
    lseek(fd, offset, SEEK_SET);
    write(fd, &color, 2);

    return 0;
}

int main(int argc, char** argv)
{
    int fbfd, status, offset;
    /* 프레임버퍼 정보 처리를 위한 구조체 */
    unsigned short pixel;

    fbfd = open(FBDEVICE, O_RDWR);    /* 사용할 프레임버퍼 장치를 오픈한다. */
    if(fbfd < 0) {
        perror("Error: cannot open framebuffer device");
        return -1;
    }

    DrawPoint(fbfd, 50, 50, makepixel(255, 0, 0));            /*  Red 점을 출력 */
    DrawPoint(fbfd, 100, 100, makepixel(0, 255, 0));        /*  Green 점을 출력 */
    DrawPoint(fbfd, 150, 150, makepixel(0, 0, 255));        /*  Blue 점을 출력 */

    close(fbfd);                                           /* 사용이 끝난 프레임버
퍼 장치를 닫는다. */

    return 0;
}

