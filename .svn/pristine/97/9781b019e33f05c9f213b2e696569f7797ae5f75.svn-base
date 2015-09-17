#include <stdio.h>
#include <unistd.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <arpa/inet.h>

#define TCP_PORT  5100

int main(int argc, char** argv)
{
  int ssock;
  int clen;
  struct sockaddr_in server_addr;
  char buf[BUFSIZ];

  if(argc < 2) {
    printf("Usage : %s IP_ADRESS\n", argv[0]);
    return -1; 
  }

  /* 소켓을 생성 */
  if((ssock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket( )");
    return -1; 
  }

  /* 소켓이 접속할 주소 지정 */
  bzero(&server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr(argv[1]);
  server_addr.sin_port = htons(TCP_PORT);

  clen = sizeof(server_addr);
  /* 지정한 주소로 접속*/
  if(connect(ssock, (struct sockaddr *)&server_addr, clen) < 0) {
    perror("connect( )");
    return -1; 
  }

  fgets(buf, BUFSIZ, stdin);
  if(write(ssock, buf, BUFSIZ) <= 0) {
    perror("write( )");
    return -1; 
  }

  shutdown(ssock, SHUT_WR);
  
  bzero(buf, BUFSIZ);
  if(read(ssock, buf, BUFSIZ) <= 0) {
    perror("read( )");
    return -1; 
  }

  shutdown(ssock, SHUT_RD);

  /* 받아온 문자열을 화면에 출력*/
  printf("Received data : %s ", buf);

  /* 소켓을 닫음*/
  close(ssock);

  return 0;
}
