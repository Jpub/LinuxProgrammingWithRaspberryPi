#include <stdio.h>
#include <strings.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SERVER_PORT 5100                                          /*서버의 포트 번호 */

int main(int argc, char** argv)
{
  int ssock;
  socklen_t clen;
  int n;
  struct sockaddr_in server_addr, client_addr;
  char buf[BUFSIZ] = "Hello world";

  fd_set readfd;                                                                 /* select( ) 함수를 위한 자료형 */
  int maxfd, client_index, start_index;
  int client_fd[5] = {0};                                                         /* 클라이언트의 소켓 FD 배열 */ 

  if((ssock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {   /* 서버 소켓 디스크립터 오픈 */
    perror("socket( )");
    return -1;
  }

  bzero(&server_addr, sizeof(server_addr));                      /* 운영체제에 서비스 등록 */
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(SERVER_PORT);
  if(bind(ssock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
    perror("bind( )");
    return -1;
  }

  if(listen(ssock, 8) < 0) {                                         /* 클라이언트의 소켓들을 위한 큐 생성 */
    perror("listen( )");
    return -1;
  }

  FD_ZERO(&readfd);                                               /* fd_set 자료형을 모두 0으로 초기화 */
  maxfd = ssock;                /* 현재 최대 파일디스크립터의 번호는 서버 소켓의 디스크립터 */
  client_index = 0;

  while(1) {
    FD_SET(ssock, &readfd);   /* 서버 소켓에서 읽기 동작 감지를 위한 fd_set 자료형 설정 */

    /* 클라이언트의 시작 주소부터 마지막 주소까지 fd_set 자료형에 설정 */
    for(start_index = 0; start_index < client_index; start_index++) {
      FD_SET(client_fd[start_index], &readfd);
      if(client_fd[start_index] > maxfd) 
        maxfd = client_fd[start_index];                           /* 가장 큰 소켓의 번호를 저장 */
    } 
    maxfd = maxfd + 1;

    /* select( ) 함수에서 읽기가 가능한 부분만 조사 */
    select(maxfd, &readfd, NULL, NULL, NULL);       /* 읽기가 가능해질때까지 블록킹 */
    if(FD_ISSET(ssock, &readfd)) {                      /* 읽기가 가능한 소켓이 서버 소켓인 경우 */
      clen = sizeof(struct sockaddr_in);                        /* 클라이언트의 요청 받아드리기 */
      int csock = accept(ssock, (struct sockaddr*)&server_addr, &clen);
      if(csock < 0) {
        perror("accept Error\n");
        return -1;
      } else { 
        FD_SET(csock, &readfd);    /* 새로 접속한 클라이언트의 소켓 번호를 fd_set에 추가 */
        client_fd[client_index] = csock;
        client_index++;
        continue;
      }

      if (client_index == 5) break;
    }

    /* 읽기 가능했던 소켓이 클라이언트였던 경우 */
    for(start_index = 0; start_index < client_index; start_index++) {
      /* for 루프를 이용해서 클라이언트들을 모두 조사 */
      if(FD_ISSET(client_fd[start_index], &readfd)) {
        bzero(buf, sizeof(buf));

        /* 해당 클라이언트에서 메시지를 읽고 다시 전송(Echo) */
        if((n = read(client_fd[start_index], buf, sizeof(buf))) > 0) {
          printf("Received data : %s", buf);
          write(client_fd[start_index], buf, n);
          close(client_fd[start_index]);     /* 클라이언트 소켓을 닫는다. */
          FD_CLR(client_fd[start_index], &readfd);
          client_index--;
        }
      }
    };
  };

  close(ssock);                                                            /* 서버 소켓을 닫는다. */

  return 0;
}

