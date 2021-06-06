#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#define TRUE 1

union semun {
    int val; /* value for SETVAL */
    struct semid_ds *buf; /* buffer for IPC_STAT, IPC_SET */
    unsigned short *array; /* array for GETALL, SETALL */
    struct seminfo *__buf; /* buffer for IPC_INFO */
}arg;
/*定义公共变量*/
int sum=0,stop_flag=0;
int sem_empty,sem_even,sem_odd;
pthread_t pth1,pth2,pth3;

/*定义信号灯P操作与V操作接口*/
void P(int semid,int index);
void V(int semid,int index);

/*定义线程函数*/
void *calcup();
void *printp_even();
void *printp_odd();

int main()
{
    /*创建信号灯*/
    sem_empty=semget(IPC_PRIVATE,1,IPC_CREAT|0666);
    sem_even=semget(IPC_PRIVATE,1,IPC_CREAT|0666);
    sem_odd=semget(IPC_PRIVATE,1,IPC_CREAT|0666);
    if(sem_even==-1||sem_empty==-1||sem_odd==-1)
        printf("Semaphore created failed!\n");

    /*信号灯赋初值*/
    arg.val=1;
    if(semctl(sem_empty,0,SETVAL,arg)==-1)
        printf("Semaphore initialized failed!\n");

    arg.val=0;
    if(semctl(sem_even,0,SETVAL,arg)==-1)
        printf("Semaphore initialized failed!\n");

    arg.val=0;
    if(semctl(sem_odd,0,SETVAL,arg)==-1)
        printf("Semaphore initialized failed!\n");

    /*创建计算线程*/
    pthread_create(&pth1,NULL,calcup,NULL);
    pthread_create(&pth2,NULL,printp_even,NULL);
    pthread_create(&pth3,NULL,printp_odd,NULL);
    printf("线程创建成功!\n");

    /*等待线程运行结束*/
    pthread_join(pth1,NULL);
    pthread_join(pth2,NULL);
    pthread_join(pth3,NULL);
    printf("线程运行结束!\n");

    /*删除信号灯*/
    semctl(sem_empty,0,IPC_RMID,arg);
    semctl(sem_even,0,IPC_RMID,arg);
    semctl(sem_odd,0,IPC_RMID,arg);
    printf("信号灯删除成功!\n");

    exit(0);
}
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

void *calcup()
{
    int add_num=1;
    stop_flag=0;  /*计算停止标志位*/
    while(add_num<=100)
    {
        P(sem_empty,0);
        /*开始计算和数*/
        sum+=add_num;
        add_num++;
        if(sum%2) V(sem_odd,0);
        else V(sem_even,0);
    }
    P(sem_empty,0);
    stop_flag=TRUE;
    V(sem_odd,0);
    V(sem_even,0);
}
void *printp_even()
{
    while(TRUE)
    {
        P(sem_even,0);
        if(stop_flag)
            break;
        printf("print even num: %d\n",sum);
        V(sem_empty,0);
    }
}
void *printp_odd()
{
    while(TRUE)
    {
        P(sem_odd,0);
        if(stop_flag)
            break;
        printf("print odd num: %d\n",sum);
        V(sem_empty,0);
    }
}











