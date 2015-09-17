#include <stdio.h> 
#include <opencv2/core/core_c.h>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/highgui/highgui_c.h>

int main(int argc, char** argv) 
{
    IplImage *image1 = cvLoadImage("sample1.jpg", CV_LOAD_IMAGE_COLOR);
    IplImage *image2 = ccvCreateImage(cvGetSize(image1), 8, 3);   
    IplImage *image3 = ccvCreateImage(cvGetSize(image1), 8, 3);   
    IplImage *image4 = ccvCreateImage(cvGetSize(image1), 8, 3);   
    IplImage *image5 = ccvCreateImage(cvGetSize(image1), 8, 3);   

    /* 윈도우를 생성한다. */    
    cvNamedWindow("IMAGE_1", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("IMAGE_2", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("IMAGE_3", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("IMAGE_4", CV_WINDOW_AUTOSIZE);

    /* 영상 처리를 진행한다. */
    cvCvtColor(image1, image2, CV_RGB2HSV);
    cvCvtPixToPlane(image2, 

    /* 화면에 처리된 결과를 표시한다. */
    cvShowImage("IMAGE_1", image1); 
    cvShowImage("IMAGE_2", image3); 
    cvShowImage("IMAGE_3", image4); 
    cvShowImage("IMAGE_4", image5); 
    
    /* 키보드가 눌려질때까지 대기한다. */
    cvWaitKey(0);

    /* 앞에서 사용할 리소드들을 해제한다. */
    cvReleaseImage(&image1); 
    cvReleaseImage(&image2);
    cvReleaseImage(&image3);
    cvReleaseImage(&image4);
    cvReleaseImage(&image5);
    
    /* 모든 윈도우를 삭제한다. */
    cvDestroyAllWindows();
    
    return 0;
}

