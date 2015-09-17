#include <wiringPi.h>
#include <stdio.h>

#define SW  5      /* GPIO24 */
#define LED 1      /* GPIO18 */

int switchControl()
{
   int i;

   pinMode(SW,  INPUT);                        /* Pin 모드를 입력으로 설정 */
   pinMode(LED, OUTPUT);

   for (i = 0; i < 10000000; i++) {
      if(digitalRead(SW) == LOW) {          /* Push 버튼이 눌러지면(LOW) */
         digitalWrite(LED, HIGH);                /* LED 켜기(On) */
         delay(1000); 
         digitalWrite(LED, LOW);                 /* LED 끄기(Off) */
      }
   };

   return 0 ;
}

int main(int argc, char** argv)
{
   wiringPiSetup();

   switchControl();                                    /* 스위치 사용을 위한 함수 */

   return 0 ;
}

