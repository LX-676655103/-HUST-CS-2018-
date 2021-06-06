#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/wait.h>
/*信号灯头文件*/
#include <sys/sem.h>
/*共享内存头文件*/
#include<sys/shm.h>
/*缓冲区大小*/
#define BUFLEN 1024
/*缓冲区个数*/
#define BUFNUM 200

#define ERROR -1
#define TRUE 1
#define STATUS_FULL 1
#define STATUS_EMPTY 0
#define STATUS_END -1

/*定义环形缓冲区结构体*/
typedef struct Buffer{
    int next_id;
    char data[BUFLEN];
    int status;
    int length;
} ShareBuffer;


/*定义信号灯P操作与V操作接口*/
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
