#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <wiringPi.h>
#include <softTone.h>
#include <errno.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <time.h>

#define	CDS	0	/* GPIO17 */	
#define LED	1	/* GPIO18 */
#define	SW	5	/* GPIO24 */
#define	SPKR	6	/* GPIO25 */
#define	MOTOR	2	/* GPIO13 */

#define TOTAL	32	/* 학교종의 전체 계이름의 수 */
#define PORT 8080

int notes[] = {		/* 학교종을 연주하기 위한 계이름 */
    391, 391, 440, 440, 391, 391, 329.63, 329.63, \
    391, 391, 329.63, 329.63, 293.66, 293.66, 293.66, 0, \
    391, 391, 440, 440, 391, 391, 329.63, 329.63, \
    391, 329.63, 293.66, 329.63, 261.63, 261.63, 261.63, 0
};

pthread_mutex_t music_lock;
pthread_mutex_t motor_lock;
static int is_run = 1; /* 스레드 종료를 위한 플래그 */

int musicPlay();
int motorControl(int);

void *musicfunction(void *arg)
{
    if(pthread_mutex_trylock(&music_lock) != EBUSY) { /* 임계 구역 설정 */
        musicPlay();
        pthread_mutex_unlock(&music_lock); /* 임계 구역 해제 */
    }

    return NULL;
}

void *motorfunction(void *arg)
{
    if(pthread_mutex_trylock(&motor_lock) != EBUSY) { /* 임계 구역 설정 */
        motorControl(MOTOR);
        pthread_mutex_unlock(&motor_lock); /* 임계 구역 해제 */
    }

    return NULL;
}

int ledControl(int gpio, int onoff)
{
    pinMode(gpio, OUTPUT) ; 			/* 핀(Pin)의 모드 설정 */
    digitalWrite(gpio, (onoff)?HIGH:LOW); 	/* LED 켜고 끄기 */

    return 0;
}

void *switchfunction(void *arg)
{
    int i;
    
    pinMode(SW, INPUT); 			/* 핀의 모드를 입력으로 설정 */
    pinMode(LED, OUTPUT);

    while(is_run) {
        if(digitalRead(SW) == LOW) { /* Push 버튼이 눌러지면(LOW) */
            digitalWrite(LED, HIGH); /* LED 켜기(On) */
        } else {
            digitalWrite(LED, LOW); /* LED 끄기(Off) */
        }
    };

    return 0;
} 

int musicPlay()
{
    int i ;

    softToneCreate(SPKR) ; 			/* 톤 출력을 위한 GPIO 설정 */
    for (i = 0; i < TOTAL; ++i) {
        softToneWrite(SPKR, notes[i]); 		/* 톤 출력 : 학교종 연주 */
        delay(280); 				/* 음의 전체 길이만큼 출력되도록 대기 */
    }

    return 0;
}

void *cdsfunction(void *arg)
{
    int i;

    pinMode(SW, INPUT); 			/* 핀의 모드를 입력으로 설정 */
    pinMode(CDS, INPUT); 			/* 핀의 모드를 입력으로 설정 */
    pinMode(LED, OUTPUT); 			/* 핀의 모드를 출력으로 설정 */

    while(is_run) {
        if(digitalRead(CDS) == HIGH) { 		/* 빛이 감지되면(HIGH) */
            digitalWrite(LED, HIGH); 		/* LED 켜기(On) */
            delay(1000);
        } else {
            digitalWrite(LED, LOW); 		/* LED 끄기(Off) */
        }
    };

    return 0;
}

int motorControl(int gpio)
{
    int i;

    pinMode(gpio, OUTPUT) ; 			/* 핀(Pin)의 모드 설정 */

    for (i = 0; i < 3; i++) {
        digitalWrite(gpio, HIGH); 		/* HIGH(1) 값을 출력 : 모터 켜기 */
        delay(1000); 				/* 1초(1000ms) 동안 대기 */
        digitalWrite(gpio, LOW); 		/* LOW(0) 값을 출력 : 모터 끄기 */
	delay(1000);
    };

    return 0;
}

int kbhit()
{
    struct termios oldt, newt; 			/* 터미널에 대한 구조체 */
    int ch, oldf;

    tcgetattr(0, &oldt); 			/* 현재 터미널에 설정된 정보를 가져온다. */
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO); 		/* 정규 모드 입력과 에코를 해제한다. */
    tcsetattr(0, TCSANOW, &newt); 		/* 새로 값으로 터미널을 설정한다. */
    oldf = fcntl(0, F_GETFL, 0);
    fcntl(0, F_SETFL, oldf | O_NONBLOCK); 	/* 입력을 논블로킹 모드로 설정한다. */

    ch = getchar();

    tcsetattr(0, TCSANOW, &oldt); 		/* 기존의 값으로 터미널의 속성을 바로 적용한다. */
    fcntl(0, F_SETFL, oldf);
    if(ch != EOF) {
        ungetc(ch, stdin); 			/* 앞에서 읽은 위치로 이전으로 포인터를 돌려준다. */
        return 1;
    }

    return 0;
}

/* 스레드 처리를 위한 함수 */
static void *clnt_connection(void *arg);
int sendData(int fd, FILE* fp, char *ct, char *file_name); 
void sendOk(FILE* fp);
void sendError(FILE* fp);

static pthread_t ptSwitch, ptCds, ptMusic, ptMotor;

void* webServer(void *arg)
{
    //int i = 0;

    int serv_sock;
    pthread_t thread;
    struct sockaddr_in serv_addr, clnt_addr;
    unsigned int clnt_addr_size;

    pthread_t ptSwitch, ptCds, ptMusic, ptMotor;
    pthread_mutex_init(&music_lock, NULL); /* 뮤텍스 초기화 */
    pthread_mutex_init(&motor_lock, NULL);

    wiringPiSetup();
    pthread_create(&ptSwitch, NULL, switchfunction, NULL);
    pthread_create(&ptCds, NULL, cdsfunction, NULL);

    /* 서버를 위한 소켓을 생성한다. */
    serv_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1) {
        perror("socket( )");
        return NULL;
    }

    /* 입력받는 포트 번호를 이용해서 서비스를 운영체제에 등록한다. */
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(PORT);
    if(bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr))==-1) {
        perror("bind( )");
        return NULL;
    }

    /* 최대 10대의 클라이언트의 동시접속을 처리할 수 있도록 큐를 생성한다. */
    if(listen(serv_sock, 10) == -1) {
        perror("listen( )");
        return NULL;
    }

    while(1) {
        int clnt_sock;

        /* 클라이언트의 요청을 기다린다. */
        clnt_addr_size = sizeof(clnt_addr);
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
        printf("Client IP : %s:%d\n", inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));

        /* 클라이언트의 요청이 들어오면 스레드를 생성하고 클라이언트의 요청을 처리한다. */
        pthread_create(&thread, NULL, clnt_connection, &clnt_sock);
        pthread_join(thread, 0);
    };

END:
    printf("Good Bye!\n");

    return NULL;
}

void *clnt_connection(void *arg)
{
     /* 스레드를 통해서 넘어온 arg를 int 형의 파일 디스크립터로 변환한다. */
    int clnt_sock = *((int*)arg), clnt_fd;
    FILE *clnt_read, *clnt_write;
    char reg_line[BUFSIZ], reg_buf[BUFSIZ];
    char method[10], ct[BUFSIZ], type[BUFSIZ];
    char file_name[256], file_buf[256];
    char* type_buf;
    int i = 0, j = 0, len = 0;

    /* 파일 디스크립터를 FILE 스트림으로 변환한다. */
    clnt_read = fdopen(clnt_sock, "r");
    clnt_write = fdopen(dup(clnt_sock), "w");
    clnt_fd = clnt_sock;

    /* 한 줄의 문자열을 읽어서 reg_line 변수에 저장한다. */
    fgets(reg_line, BUFSIZ, clnt_read);

    /* reg_line 변수에 문자열을 화면에 출력한다. */
    fputs(reg_line, stdout);

    /* ‘ ’ 문자로 reg_line을 구분해서 요청 라인의 내용(메소드)를 분리한다. */
    strcpy(method, strtok(reg_line, " "));
    if(strcmp(method, "POST") == 0) { /* POST 메소드일 경우를 처리한다. */
        sendOk(clnt_write); /* 단순히 OK 메시지를 클라이언트로 보낸다. */
        fclose(clnt_read);
        fclose(clnt_write);

        return (void*)NULL;
    } else if(strcmp(method, "GET") != 0) { /* GET 메소드가 아닐 경우를 처리한다. */
        sendError(clnt_write); /* 에러 메시지를 클라이언트로 보낸다. */
        fclose(clnt_read);
        fclose(clnt_write);

        return (void*)NULL;
    }

    strcpy(file_name, strtok(NULL, " ")); /* 요청 라인에서 경로(path)를 가져온다. */
    if(file_name[0] == '/') { /* 경로가 ‘/’로 시작될 경우 /를 제거한다. */
        for(i = 0, j = 0; i < BUFSIZ; i++) {
            if(file_name[0] == '/') j++;
            file_name[i] = file_name[j++];
            if(file_name[i+1] == '\0') break;
        };
    }

    /* 라즈베리 파이를 제어하기 위한 HTML 코드를 분석해서 처리한다. */
    if(strstr(file_name, "?") != NULL) {
        char optLine[32];
        char optStr[4][16];
        char opt[8], var[8];
        char* tok;
        int i, count = 0;

        strcpy(file_name, strtok(file_name, "?"));
        strcpy(optLine, strtok(NULL, "?"));

        /* 옵션을 분석한다. */
        tok = strtok(optLine, "&");
        while(tok != NULL) {
            strcpy(optStr[count++], tok);
            tok = strtok(NULL, "&");
        };

        /* 분석한 옵션을 처리한다. */
        for(i = 0; i < count; i++) {
            strcpy(opt, strtok(optStr[i], "="));
            strcpy(var, strtok(NULL, "="));
            printf("%s = %s\n", opt, var);
            if(!strcmp(opt, "led") && !strcmp(var, "On")) { /* LED를 켠다. */
                ledControl(LED, 1);
            } else if(!strcmp(opt, "led") && !strcmp(var, "Off")) { /* LED를 끈다. */
                ledControl(LED, 0);
            } else if(!strcmp(opt, "music") && !strcmp(var, "Play")) { /* 음악을 연주한다. */
                pthread_create(&ptMusic, NULL, musicfunction, NULL);
            } else if(!strcmp(opt, "motor") && !strcmp(var, "On")) { /* 모터를 켠다. */
                pthread_create(&ptMotor, NULL, motorfunction, NULL);
            }
        };
    }

    /* 메시지 헤더를 읽어서 화면에 출력하고 나머지는 무시한다. */
    do {
        fgets(reg_line, BUFSIZ, clnt_read);
        fputs(reg_line, stdout);
        strcpy(reg_buf, reg_line);
        type_buf = strchr(reg_buf, ':');
    } while(strncmp(reg_line, "\r\n", 2)); /* 요청 헤더는 ‘\r\n’으로 끝난다. */

    /* 파일의 이름을 이용해서 클라이언트로 파일의 내용을 보낸다. */
    strcpy(file_buf, file_name);
    sendData(clnt_fd, clnt_write, ct, file_name);

    fclose(clnt_read); /* 파일의 스트림을 닫는다. */
    fclose(clnt_write);

    pthread_exit(0); /* 스레드를 종료시킨다. */

    return (void*)NULL;
}

/* 외부 파일에 있는 이미지를 캡처하는 함수를 선언한다. */
extern "C" int captureImage(int fd);

int sendData(int fd, FILE* fp, char *ct, char *file_name)
{
    /* 클라이언트로 보낼 성공에 대한 응답 메시지 */
    char protocol[ ] = "HTTP/1.1 200 OK\r\n";
    char server[ ] = "Server:Netscape-Enterprise/6.0\r\n";
    char cnt_type[ ] = "Content-Type:text/html\r\n";
    char end[ ] = "\r\n"; /* 응답 헤더의 끝은 항상 \r\n */
    char buf[BUFSIZ];
    int len;

    fputs(protocol, fp);
    fputs(server, fp);
    fputs(cnt_type, fp);
    fputs(end, fp);
    fflush(fp);

    /* 파일 이름이 capture.bmp인 경우 이미지를 캡처한다. */ 
    if(!strcmp(file_name, "capture.bmp"))
        captureImage(fd);

    fd = open(file_name, O_RDWR); /* 파일을 연다. */
    do {
        len = read(fd, buf, BUFSIZ); /* 파일을 읽어서 클라이언트로 보낸다. */
        fwrite(buf, len, sizeof(char), fp);
    } while(len == BUFSIZ);

    fflush(fp);

    close(fd); /* 파일을 닫는다. */

    return 0;
}

void sendOk(FILE* fp)
{
    /* 클라이언트에 보낼 성공에 대한 HTTP 응답 메시지 */
    char protocol[ ] = "HTTP/1.1 200 OK\r\n";
    char server[ ] = "Server: Netscape-Enterprise/6.0\r\n\r\n";

    fputs(protocol, fp);
    fputs(server, fp);
    fflush(fp);
}

void sendError(FILE* fp)
{
    /* 클라이언트로 보낼 실패에 대한 HTTP 응답 메시지 */
    char protocol[ ] = "HTTP/1.1 400 Bad Request\r\n";
    char server[ ] = "Server: Netscape-Enterprise/6.0\r\n";
    char cnt_len[ ] = "Content-Length:1024\r\n";
    char cnt_type[ ] = "Content-Type:text/html\r\n\r\n";

    /* 화면에 표시될 HTML의 내용 */
    char content1[ ] = "<html><head><title>BAD Connection</tiitle></head>";
    char content2[ ] = "<body><font size=+5>Bad Request</font></body></html>";

    printf("send_error\n");
    fputs(protocol, fp);
    fputs(server, fp);
    fputs(cnt_len, fp);
    fputs(cnt_type, fp);
    fputs(content1, fp);
    fputs(content2, fp);
    fflush(fp);
}

#include <QApplication>
#include <QLabel>
#include <QRadioButton>
#include <QButtonGroup>
#include <QPushButton>
#include <QHBoxLayout>
#include <QGridLayout>

#include "withPi.h"

static pthread_t ptWebServer; /* 웹 서버의 쓰래드 생성을 위해서 설정 */

PiWidget::PiWidget(QWidget* parent) : QWidget(parent)
{
    QLabel* labelLED = new QLabel("LED", this);

    /* 라디오 버튼 위젯을 배치하고 수평으로 레이아웃(QHBoxLayout) 설정 */
    QRadioButton* radioButtonOn = new QRadioButton("O&n", this);
    QRadioButton* radioButtonOff = new QRadioButton("O&ff", this);
    QHBoxLayout* horizontalLayout = new QHBoxLayout();
    horizontalLayout->addWidget(radioButtonOn);
    horizontalLayout->addWidget(radioButtonOff);

    /* 레이블과 푸시 버튼을 생성 */
    QLabel* labelMusic = new QLabel("Music", this);
    QPushButton* pushButtonPlay = new QPushButton("&Play", this);
    QLabel* labelMotor = new QLabel("Motor", this);
    QPushButton* pushButtonMotor = new QPushButton("&On", this);

    /* 각 위젯들과 QHBoxLayout 클래스를 QGridLayout으로 레아이웃 */
    QGridLayout* gridLayout = new QGridLayout(this);
    gridLayout->addWidget(labelLED, 0, 0, 1, 1);
    gridLayout->addLayout(horizontalLayout, 0, 1, 1, 1);
    gridLayout->addWidget(labelMusic, 1, 0, 1, 1);
    gridLayout->addWidget(pushButtonPlay, 1, 2, 1, 1);
    gridLayout->addWidget(labelMotor, 2, 0, 1, 1);
    gridLayout->addWidget(pushButtonMotor, 2, 2, 1, 1);

    /* 각 위젯들과 사용자 정의 슬롯을 연결 */
    connect(radioButtonOn, SIGNAL(clicked( )), SLOT(ledOn( )));
    connect(radioButtonOff, SIGNAL(clicked( )), SLOT(ledOff( )));
    connect(pushButtonPlay, SIGNAL(clicked( )), SLOT(musicPlay( )));
    connect(pushButtonMotor, SIGNAL(clicked( )), SLOT(motorOn( )));

    /* 위의 두 레이아웃 중 QGridLayout을 폼의 레이아웃으로 설정 */
    setLayout(gridLayout);
}

void PiWidget::ledOn( ) /* LED를 켠다. */
{
    ledControl(LED, 1);
}

void PiWidget::ledOff( ) /* LED를 끈다. */
{
    ledControl(LED, 0);
}

void PiWidget::musicPlay( ) /* 음악을 연주한다. */
{
    pthread_create(&ptMusic, NULL, musicfunction, NULL);
}

void PiWidget::motorOn( ) /* 모터를 켠다 */
{
    pthread_create(&ptMotor, NULL, motorfunction, NULL);
}

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    PiWidget* widget = new PiWidget();
    widget->resize(150, 250);
    widget->show();

    /* 웹 서버의 실행을 위한 쓰레드를 생성한다. */
    pthread_create(&ptWebServer, NULL, webServer, NULL);

    return app.exec();
}
