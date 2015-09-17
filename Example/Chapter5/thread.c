#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int cnt = 0;
static int semid;

void p( )                                                          /* 세마포어의 P 연산 */
{
  struct sembuf pbuf;
  pbuf.sem_num = 0;
  pbuf.sem_op = -1; 
  pbuf.sem_flg = SEM_UNDO;
  if(semop(semid, &pbuf, 1) == -1)                 /* 세마포어의 감소 연산을 수행한다. */
     perror("p : semop()");
}

void v( )                                                          /* 세마포어의 V 연산 */
{
  struct sembuf vbuf;
  vbuf.sem_num = 0;
  vbuf.sem_op = 1;
  vbuf.sem_flg = SEM_UNDO;
  if(semop(semid, &vbuf, 1) == -1)                 /* 세마포어의 증가 연산을 수행한다. */
     perror("v : semop()");
}

void *ptheadV(void *arg)                                /* V 연산을 수행하기 위한 함수를 작성한다. */
{
  int i;
  for(i = 0; i < 10; i++) {
    if(cnt >= 7) usleep(100);                            /* 7이상이면 100ms 동안 대기한다. */
    cnt++;
    v();
    printf("increase : %d\n", cnt) ;
  }
  
  return NULL; 
}

void *ptheadP(void *arg)                                /* P 연산을 수행하기 위한 함수를 작성한다. */
{
  int i;

  for(i = 0; i < 10; i++) {
    p();                                                            /* 세마포어가 0이 되면 블럭된다. */
    cnt--;
    printf("decrease : %d\n", cnt);
    usleep(50);                                                 /* 50ms 간 기다린다. */
  }

  return NULL;
}

int main(int argc, char *argv[])
{
   pthread_t ptV, ptP;                                      /* 쓰레드를 위한 자료형 */
   union semun {                                             /* semun 공용체 */
      int val;
      struct semid_ds *buf;
      unsigned short int  *arrary;
   } arg;


  /* 세마포어에 대한 채널 얻기 */
  if((semid = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666)) == -1) {
     perror("semget( )");
     return -1;
  }

  arg.val = 1;                                                   /* 세마포어 값을 1로 >설정 */
  if(semctl(semid, 0, SETVAL, arg) == -1) {
     perror("semctl() : SETVAL");
     return -1;
  }

  pthread_create(&ptV, NULL, ptheadV, NULL);    /* 쓰레드 생성 */
  pthread_create(&ptP, NULL, ptheadP, NULL);
  pthread_join(ptV, NULL);                             /* 쓰레드가 종료될 때까지 대기 */
  pthread_join(ptP, NULL);

  /* 세마포어에 대한 채널 삭제 */
  if(semctl(semid, 0, IPC_RMID, arg) == -1) {
     perror("semctl( ) : IPC_RMID");
     return -1;
  }

  return 0;
}

