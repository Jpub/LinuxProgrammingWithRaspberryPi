#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <arpa/inet.h>

#define TCP_PORT  5100                                 /* 서버의 포트 번호 */

int main(int argc, char** argv)
{
   int ssock;                                               /* 소켓 디스크립트 정의*/
   socklen_t clen;
   int n;
   struct sockaddr_in client_addr, server_addr;    /* 주소 구조체 정의 */
   char buf[BUFSIZ] = "Hello world";                     /* 클라이언트에 보내줄 문자열 */

   /* 서버 소켓 생성*/
   if((ssock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
      perror("socket( )");
      return -1; 
   }   

   /* 주소 구조체에 주소 지정*/
   bzero(&server_addr, sizeof(server_addr));
   server_addr.sin_family = AF_INET;
   server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
   server_addr.sin_port = htons(TCP_PORT);    /* 사용할 포트 지정 */
    
   // bind 함수를 사용하여 서버 소켓의 주소 설정
   if(bind(ssock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
     perror("bind( )");
     return -1; 
   }   
    
   /* 지정한 주소에서 클라이언트 접속을 기다림*/
   if(listen(ssock, 8) < 0) {
     perror("listen( )");
     return -1; 
   }   

   clen = sizeof(client_addr);
   while(1) {
     /* 클라이언트가 접속하면 접속을 허용하고 클라이언트 소켓 생성 */
     int csock = accept(ssock, (struct sockaddr *)&client_addr, &clen);

     printf("Client is connected : %s\n", inet_ntoa(client_addr.sin_addr));
    
     if((n = read(csock, buf, BUFSIZ)) <= 0)
       perror("read( )");

     printf("Received data : %s", buf);

     /* 클라이언트로 buf에 있는 문자열 전송*/
     if(write(csock, buf, n) <= 0)
       perror("write( )");

     close(csock); /* 클라이언트 소켓을 닫음*/
   }   

   close(ssock);             /* 서버 소켓을 닫음 */

   return 0;
}

