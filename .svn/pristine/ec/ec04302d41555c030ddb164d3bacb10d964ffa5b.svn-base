#ifndef __BMP_FILE_H__
#define __BMP_FILE_H__

typedef struct  __attribute__((__packed__)) {
    unsigned char bfType[2];                  /* BM 표시 : "BM" (2 글자) 문자 */
    unsigned int bfSize;                       /* 파일의 크기 : 4 바이트 정수  */
    unsigned short bfReserved1;        /* 추후의 확장을 위해서 필드(reserved) */
    unsigned short bfReserved2;        /* 추후의 확장을 위해서 필드(reserved) */
    unsigned int bfOffBits;                   /* 실제 이미지까지의 오프셋(offset) : 바이트 */
} BITMAPFILEHEADER;

typedef struct {
    unsigned int biSize;                       /* 현 구조체의 크기 : 4바이트 */
    unsigned int biWidth;                     /* 이미지의 폭(픽셀단위) : 4바이트 */
    unsigned int biHeight;                    /* 이미지의 높이(픽셀단위) : 4바이트 */
    unsigned short biPlanes;               /* 비트 플레인 수 (항상1) : 2바이트 */
    unsigned short biBitCount;            /* 픽셀 당 비트 수 : 2바이트 */
    unsigned int biCompression;         /* 압축 유형 : 4바이트 */
    unsigned int SizeImage;                /* 이미지의 크기(압축 전의 바이트 단위) : 4바이트 */
    unsigned int biXPelsPerMeter;      /* 가로 해상도  : 4바이트 */
    unsigned int biYPelsPerMeter;      /* 세로 해상도 : 4바이트 */
    unsigned int biClrUsed;                 /* 실제 사용되는 색상수 : 4바이트 */
    unsigned int biClrImportant;          /* 중요한 색상 인덱스(0 인 경우 전체) : 4바이트 */
} BITMAPINFOHEADER;

typedef struct {
    unsigned char rgbBlue;
    unsigned char rgbGreen;
    unsigned char rgbRed;
    unsigned char rgbReserved;
} RGBQUAD;

#endif /* __BMP_FILE_H__ */

