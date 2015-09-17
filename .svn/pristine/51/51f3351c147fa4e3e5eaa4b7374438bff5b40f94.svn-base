#include <wiringPi.h>
#include <stdio.h>

int fndControl(int num)
{
   int  i;
   int gpiopins[4] = {4, 1, 16, 15};     /* A, B, C, D : 23 18 15 14 */
   int number[10][4] = { {0,0,0,0},      /* 0 */
                         {0,0,0,1},      /* 1 */
                         {0,0,1,0},      /* 2 */
                         {0,0,1,1},      /* 3 */
                         {0,1,0,0},      /* 4 */
                         {0,1,0,1},      /* 5 */
                         {0,1,1,0},      /* 6 */
                         {0,1,1,1},      /* 7 */
                         {1,0,0,0},      /* 8 */
                         {1,0,0,1} };    /* 9 */

   for (i = 0; i < 4; i++) {
       pinMode(gpiopins[i], OUTPUT) ;                /* 모든 Pin의 출력 설정 */
   }

   for (i = 0; i < 4; i++) {
      digitalWrite(gpoipins[i], number[num][i]?HIGH:LOW);         /* HIGH(1) 값을 출력 : LED 켜기 */
   };
   delay(1000);                                                   /* 1초(1000ms) 동안 대기 */

   for (i = 0; i < 4; i++) {
      digitalWrite(number[num][i], LOW);           /* LOW (0) 값을 출력 : LED끄기*/
   };
   getchar();

   return 0;
}

int main(int argc, char** argv)
{
   int no; 

   if(argc < 2) {
      printf("Usage : %s NO\n", argv[0]);
      return -1;
   }
   no = atoi(argv[1]);
   wiringPiSetup();                                             /* wiringPi 초기화 */

   fndControl(no);

   return 0;
}

