#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/mman.h>

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

static int DrawLine(int fd, int start_x, int end_x, int y, unsigned short color) 
{
    int x, offset;
    struct fb_var_screeninfo vinfo;

    /* 현재 프레임버퍼에 대한 고정된 화면 정보를 얻어온다. */
    if(ioctl(fd, FBIOGET_VSCREENINFO, &vinfo) < 0) {
        perror("Error reading fixed information");
        return -1;
    }

    for(x = start_x; x < end_x; x++) {
        offset = (x+y*vinfo.xres)*2;
        lseek(fd, offset, SEEK_SET);
        write(fd, &color, 2);
    };

    return 0;
}

void DrawCircle(int fd, int center_x, int center_y, int radius, unsigned short color)
{
    int x = radius, y = 0;
    int radiusError = 1 - x;
 
    while(x >= y) {
        DrawPoint(fd,  x + center_x,  y + center_y, color);
        DrawPoint(fd,  y + center_x,  x + center_y, color);
        DrawPoint(fd, -x + center_x,  y + center_y, color);
        DrawPoint(fd, -y + center_x,  x + center_y, color);
        DrawPoint(fd, -x + center_x, -y + center_y, color);
        DrawPoint(fd, -y + center_x, -x + center_y, color);
        DrawPoint(fd,  x + center_x, -y + center_y, color);
        DrawPoint(fd, y + center_x, -x + center_y, color);

        y++;
        if (radiusError < 0) {
            radiusError += 2 * y + 1;
        } else {
            x--;
            radiusError += 2 * (y - x + 1);
        }
    };
}

static int DrawFace(int fd, int start_x, int start_y, int end_x, int end_y, unsigned short color) 
{
    int x, y, offset;
    struct fb_var_screeninfo vinfo;

    /* 현재 프레임버퍼에 대한 고정된 화면 정보를 얻어온다. */
    if(ioctl(fd, FBIOGET_VSCREENINFO, &vinfo) < 0) {
        perror("Error reading fixed information");
        return -1;
    }

    if(end_x == 0) end_x = vinfo.xres;
    if(end_y == 0) end_y = vinfo.yres;

    for(x = start_x; x < end_x; x++) {
        for(y = start_y; y < end_y; y++) {
            offset = (x + y*vinfo.xres)*2;
            lseek(fd, offset, SEEK_SET);
            write(fd, &color, 2);
        };
    };

    return 0;
}

static int DrawFaceMMAP(int fd, int start_x, int start_y, int end_x, int end_y, unsigned short color)
{
    int x, y, offset;
    struct fb_var_screeninfo vinfo;
    unsigned short *pfb;

    /* 현재 프레임버퍼에 대한 고정된 화면 정보를 얻어온다. */
    if(ioctl(fd, FBIOGET_VSCREENINFO, &vinfo) < 0) {
        perror("Error reading fixed information");
        return -1;
    }

    if(end_x == 0) end_x = vinfo.xres;
    if(end_y == 0) end_y = vinfo.yres;
    pfb = (unsigned short *)mmap(0, vinfo.xres*vinfo.yres*2, PROT_READ | PROT_WRITE,
                                                     MAP_SHARED, fd, 0);

    for(x = start_x; x < end_x; x++) {
        for(y = start_y; y < end_y; y++) {
            *(pfb + x + y * vinfo.xres) = color;                  //            pfb[x + y * vinfo.xres] = color;
        };
    };

    munmap(pfb, vinfo.xres*vinfo.yres*2);

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

//    DrawFace(fbfd, 0, 0, 0, 0, makepixel(255, 255, 0)) ;         /* Yellow색상을 생성 */ 
    DrawFaceMMAP(fbfd, 0, 0, 0, 0, makepixel(255, 255, 0)) ;         /* Yellow색상을 생성 */ 

    DrawPoint(fbfd, 50, 50, makepixel(255, 0, 0));            /*  Red 점을 출력 */
    DrawPoint(fbfd, 100, 100, makepixel(0, 255, 0));        /*  Green 점을 출력 */
    DrawPoint(fbfd, 150, 150, makepixel(0, 0, 255));        /*  Blue 점을 출력 */

    DrawLine(fbfd, 0, 100, 200, makepixel(0, 255, 255)) ;          /* Cyan 색상을 생성 */ 

    DrawCircle(fbfd, 200, 200, 100, makepixel(255, 0, 255)) ;         /* Magenta색상을 생성 */ 

    close(fbfd);                                           /* 사용이 끝난 프레임버
퍼 장치를 닫는다. */

    return 0;
}

