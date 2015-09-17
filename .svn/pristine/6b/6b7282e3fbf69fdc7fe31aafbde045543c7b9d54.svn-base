#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>

#define  FBDEV                      "/dev/fb0"
#define  CAMERA_COUNT    3
#define  CAM_WIDTH             640
#define  CAM_HEIGHT           480

using namespace cv;

const static char* cascade_name = 
                  "/usr/share/opencv/haarcascades/haarcascade_frontalface_alt.xml";

int main(int argc, char** argv)
{
    int fbfd;
    /* 프레임버퍼 정보 처리를 위한 구조체 */
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;

    unsigned char *buffer, r, g, b;
    unsigned int x, y, t, i, j, screensize;
    unsigned short *pfbmap, *pOutdata, pixel;
    CvCapture* capture;                                       /* 카메라를 위한 변수 */
    CascadeClassifier cascade;                           /* 얼굴 인식을 위한 classifier */
    IplImage *frame;                                             /* 영상을 위한 변수 */
    CvPoint pt1, pt2;                         /* 인식된 얼굴의 대각선의 두 점의 저장을 위한 변수들 */

    /* 얼굴 인식을 위한 캐스캐이트를 불러온다. */
    if(!cascade.load(cascade_name)) {
        perror("load( )");
        return -1;
    }

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

        /* 카메라 캡처를 위해서 영상을 다른 이름으로 복사한다. */
        Mat image;
        cvtColor(Mat(frame), image, CV_BGR2GRAY);
        equalizeHist(image, image);

        /* 영상에서 얼굴들을 인식한다. */
        std::vector<Rect> faces;
        cascade.detectMultiScale(image, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, 
                                                         Size(30, 30));

        /* 영상에서 찾은 얼굴들을 프레임에 표시한다. */
        for(j = 0; j < faces.size(); j++) {
              /* 찾은 얼굴의 양 모서리 구하기 */
             pt1.x = faces[j].x; pt2.x = (faces[j].x + faces[j].width);
             pt1.y = faces[j].y; pt2.y = (faces[j].y + faces[j].height);

             /* 이미지에 얼굴 그리기 */
             cvRectangle(frame, pt1, pt2, CV_RGB(255,0,0), 3, 8, 0);
        };

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
            };
        };

        memcpy(pfbmap, pOutdata, screensize);
    };

    /* 사용이 끝난 자원과 메모리를 해제한다. */
    munmap(pfbmap, screensize);
    free(pOutdata);

    close(fbfd);
    return 0;
}

