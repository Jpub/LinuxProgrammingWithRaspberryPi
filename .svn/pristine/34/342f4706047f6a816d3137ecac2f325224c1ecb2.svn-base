#include <stdio.h>
#include <string.h>

#include "bmpHeader.h" 

/* BMP의 헤더를 생성한다. */
int makeBmpHeader(int fd, int width, int height)
{
    int dataSize, fileSize;

    /* BITMAPFILEHEADER */
    unsigned char header[14] = {
        'B','M',                                /* BM 표시 : "BM" (2 글자) 문자 */
        0,0,0,0,                              /* 파일의 크기 : 4 바이트 정수  */
        0,0,                                    /* 추후의 확장을 위해서 필드(reserved) */
        0,0,                                    /* 추후의 확장을 위해서 필드(reserved) */
        40+14,0,0,0                       /* 실제 이미지까지의 오프셋(offset) : 바이트 */
    };

   /* BITMAPINFOHEADER */
    unsigned char info[40] = {
        40,0,0,0,                            /* 현 구조체의 크기 : 4바이트 */
        0,0,0,0,                              /* 이미지의 폭(픽셀단위) : 4바이트 */
        0,0,0,0,                              /* 이미지의 높이(픽셀단위) : 4바이트 */
        1,0,                                    /* 비트 플레인 수 (항상1) : 2바이트 */
        24,0,                                  /* 픽셀 당 비트 수 : 2바이트 */
        0,0,0,0,                              /* 압축 유형 : 4바이트 */
        0,0,0,0,                              /* 이미지의 크기(압축 전의 바이트 단위) : 4바이트 */
        0x12,0x0B,0,0,                  /* 가로 해상도 : 4바이트 */
        0x12,0x0B,0,0,                  /* 세로 해상도 : 4바이트 */
        0,0,0,0,                              /* 실제 사용되는 색상수 : 4바이트 */
        0,0,0,0,                              /* 중요한 색상 인덱스(0 인 경우 전체) : 4바이트 */
    };

    dataSize = width*height*3;                                          /* 데이터의 크기 */
    fileSize = dataSize + sizeof(header) + sizeof(info);     /* 파일의 크기 */

    if(fd <= 0 || width <= 0 || height <= 0) return -1;           /* 잘못된 데이터 값인지 검사 */

    /* 파일의 크기 : 4 바이트 정수  */
    header[2] = (unsigned char)(fileSize        );
    header[3] = (unsigned char)(fileSize>>  8);
    header[4] = (unsigned char)(fileSize>>16);
    header[5] = (unsigned char)(fileSize>>24);

    /* 이미지의 폭(픽셀단위) : 4바이트 */
    info[4] = (unsigned char)(width        );
    info[5] = (unsigned char)(width>>  8);
    info[6] = (unsigned char)(width>>16);
    info[7] = (unsigned char)(width>>24);

    /* 이미지의 높이(픽셀단위) : 4바이트 */
    info[  8] = (unsigned char)(height        );
    info[  9] = (unsigned char)(height>>  8);
    info[10] = (unsigned char)(height>>16);
    info[11] = (unsigned char)(height>>24);

    /* 이미지의 크기(압축 전의 바이트 단위) : 4바이트 */
    info[24] = (unsigned char)(dataSize       );   
    info[25] = (unsigned char)(dataSize>> 8);
    info[26] = (unsigned char)(dataSize>>16);
    info[27] = (unsigned char)(dataSize>>24);

    write(fd, (char*)header, sizeof(header));                     /* BMP 헤더를 저장한다. */
    write(fd, (char*)info, sizeof(info));                                /* BMP의 정보를 저장한다. */

    return 0;
}

