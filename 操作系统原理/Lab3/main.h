#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/wait.h>
/*�źŵ�ͷ�ļ�*/
#include <sys/sem.h>
/*�����ڴ�ͷ�ļ�*/
#include<sys/shm.h>
/*��������С*/
#define BUFLEN 1024
/*����������*/
#define BUFNUM 200

#define ERROR -1
#define TRUE 1
#define STATUS_FULL 1
#define STATUS_EMPTY 0
#define STATUS_END -1

/*���廷�λ������ṹ��*/
typedef struct Buffer{
    int next_id;
    char data[BUFLEN];
    int status;
    int length;
} ShareBuffer;


/*�����źŵ�P������V�����ӿ�*/
union semun {
    int val; /* value for SETVAL */
    struct semid_ds *buf; /* buffer for IPC_STAT, IPC_SET */
    unsigned short *array; /* array for GETALL, SETALL */
    struct seminfo *__buf; /* buffer for IPC_INFO */
}arg;


void P(int semid,int index)
{
    struct sembuf sem;
    sem.sem_num=index;
    sem.sem_op=-1;
    sem.sem_flg=0;
    semop(semid,&sem,1);
}
void V(int semid,int index)
{
    struct sembuf sem;
    sem.sem_num=index;
    sem.sem_op=1;
    sem.sem_flg=0;
    semop(semid,&sem,1);
}
