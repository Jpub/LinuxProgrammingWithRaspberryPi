#include <stdio.h> 
#include <fcntl.h>
#include <unistd.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include <opencv2/core/core_c.h>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/highgui/highgui_c.h>

#define  FBDEV                      "/dev/fb0"
#define  CAMERA_COUNT    100
#define  CAM_WIDTH             640
#define  CAM_HEIGHT           480

int main(int argc, char** argv)
{
    int fbfd;
    /* 프레임버퍼 정보 처리를 위한 구조체 */
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;

    unsigned char *buffer, r, g, b;
    unsigned int x, y, t, i, screensize;
    unsigned short *pfbmap, *pOutdata, pixel;
    CvCapture* capture;                                       /* 카메라를 위한 변수 */ 
    IplImage *frame;                                             /* 영상을 위한 변수 */

    /* 캡쳐한 카메라를 지정한다. */
    capture = cvCaptureFromCAM(0);
    if(capture == 0) {
        perror("OpenCV : open WebCam\n");
        return -1;
    }

    /* 캡쳐할 영상의 속성을 설정한다. */
    cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, CAM_WIDTH);
    cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, CAM_HEIGHT);

    /* 프레임버퍼 열기 */
    fbfd = open(FBDEV, O_RDWR);
    if(fbfd == -1) {
        perror("open( ) : framebuffer device");
        return EXIT_FAILURE;
    }

    /* 프레임버퍼의 정보 가져오기 */
    if(ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
         perror("Error reading variable information.");
         exit(EXIT_FAILURE);
    }

    /* mmap( ) : 프레임버퍼를 위한 메모리 공간 확보 */
    screensize = vinfo.xres * vinfo.yres * 2;
    pfbmap = (unsigned short *)mmap(NULL, screensize, PROT_READ | PROT_WRITE,
                                                                    MAP_SHARED, fbfd, 0);
    if((int)pfbmap == -1) {
        perror("mmap() : framebuffer device to memory");
        return EXIT_FAILURE;
    }
    memset(pfbmap, 0, screensize);

    /* 출력할 영상을 위한 공간 확보 */
    pOutdata = (unsigned short*)malloc(screensize);
    memset(pOutdata, 0, screensize);
    for(i=t=b=g=r=0; i < CAMERA_COUNT; i++) {
        cvGrabFrame(capture);                          /* 카메라로 부터 한장의 영상을 가져온다. */
        frame = cvRetrieveFrame(capture);        /* 카메라 영상에서 이미지 데이터를 획득 */ 
        buffer = (uchar*)frame->imageData;       /* IplImage 클래스의 영상 데이터 획득 */

        /* 프레임 버퍼로 출력 */
        for(x = 0; x < frame->height; x++) {
            t = x * frame->width;
            for(y = 0; y < frame->width; y++) {
                r = *(buffer + (t + y)*3 + 2);
                g = *(buffer + (t + y)*3 + 1);
                b = *(buffer + (t + y)*3 + 0);
                pixel = ((r>>3)<<11)|((g>>2)<<5)|(b>>3);

                pOutdata[y+t+(vinfo.xres-frame->width)*x] = pixel;
            }
        }

        memcpy(pfbmap, pOutdata, screensize);
    };

    /* 사용이 끝난 자원과 메모리를 해제한다. */
    munmap(pfbmap, frame->width*frame->height*2);
    free(pOutdata);

    close(fbfd);
    return 0;
}

