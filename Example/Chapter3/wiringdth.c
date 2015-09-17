#include <wiringPi.h>
#include <stdio.h>

#define MAX_COUNT 85 
#define DHT_PIN 1					/* GPIO 핀의 번호 */

/* 온습도값의저장을위한전역변수 */ 
int dhtVal[5] = {0, 0, 0, 0, 0};

void readData(void)
{
    unsigned short state = HIGH; 
    unsigned short counter = 0; 
    unsigned short j = 0, i; 
    float farenheit;
        
    for(i = 0; i < 5; i++) dhtVal[i]=0;

    /* 값측정을위한핀설정 */ 
    pinMode(DHT_PIN, OUTPUT); 
    digitalWrite(DHT_PIN, LOW); 
    delay(18); 
    digitalWrite(DHT_PIN, HIGH); 
    delayMicroseconds(40); 
    pinMode(DHT_PIN, INPUT);

    for(i = 0; i < MAX_COUNT; i++) { 
        counter = 0;

        while(digitalRead(DHT_PIN) == state) { 
            counter++;
            delayMicroseconds(1);
            if(counter == 255) break; 
        };

        state = digitalRead(DHT_PIN); 			/* 값 읽어 오기 */
        if(counter == 255) break;

        /* 상위3개의변화량은무시 */ 
        if((i >= 4) && (i%2 == 0)) {
            dhtVal[j/8] <<= 1;
            if(counter > 16) dhtVal[j/8] |= 1; 
            j++;
        } 
    }

    /* 체크섬 검사 측정 값 출력 */
    if((j >= 40) && (dhtVal[4] == ((dhtVal[0] + dhtVal[1] + dhtVal[2] + dhtVal[3]) & 0xFF))) { 
        farenheit = dhtVal[2]*9./5.+32;
        printf("Humidity = %d.%d %% Temperature = %d.%d *C (%.1f *F)\n", dhtVal[0], dhtVal[1], dhtVal[2], dhtVal[3], farenheit);
    } else printf("Invalid Data!!\n"); }


int main(int argc, char** argv)
{
    if(wiringPiSetup() == -1) return -1;

    /* 온습도 측정을 위한 순환 루프 */ 
    while(1) {
        readData();
        delay(3000); 
    };
    
    return 0; 
}