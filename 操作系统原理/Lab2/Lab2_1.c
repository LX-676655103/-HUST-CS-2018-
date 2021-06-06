#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

/*定义互斥信号灯&机票总数变量*/
#define TICK_NUM 200
int ticket_num=TICK_NUM;
int mutex_id;
pthread_t pth1,pth2,pth3;

union semun {
    int val; /* value for SETVAL */
    struct semid_ds *buf; /* buffer for IPC_STAT, IPC_SET */
    unsigned short *array; /* array for GETALL, SETALL */
    struct seminfo *__buf; /* buffer for IPC_INFO */
}arg;

/*定义信号灯P操作与V操作接口*/
void P(int semid,int index);
void V(int semid,int index);

/*定义售票线程函数*/
void *subp();

int main()
{
    /*创建互斥信号灯*/
    mutex_id=semget(IPC_PRIVATE,1,IPC_CREAT|0666);
    if(mutex_id==-1) printf("Semaphore created failed!\n");

    /*信号灯赋初值*/
    arg.val=1;
    if(semctl(mutex_id,0,SETVAL,arg)==-1)
        printf("Semaphore initialized failed!\n");

    /*创建3个售票线程*/
    pthread_create(&pth1,NULL,subp,NULL);
    pthread_create(&pth2,NULL,subp,NULL);
    pthread_create(&pth3,NULL,subp,NULL);

    /*等待线程运行结束*/
    pthread_join(pth1,NULL);
    pthread_join(pth2,NULL);
    pthread_join(pth3,NULL);
    printf("线程运行结束!\n");

    /*删除信号灯*/
    semctl(mutex_id,0,IPC_RMID,arg);
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
void *subp()
{
    int sold_num=0;
    pthread_t tid=pthread_self();
    /*售票中*/
    while(1)
    {
        usleep(10000);
        P(mutex_id,0);
        if(ticket_num)
        {
            sold_num++;
            printf("No.%ld Ticket Machine: No.%d Ticket successfully booked!\n",tid,TICK_NUM-ticket_num+1);
            printf("Currently remaining ticket: %d\n",ticket_num);
            ticket_num--;
            V(mutex_id,0);
        }
        else
        {
            V(mutex_id,0);
            break;
        }
    }
    /*售票结束，线程退出*/
    printf("No.%ld Ticket Machine sold %d tickets!\n",tid,sold_num);
    //pthread_exit(0);
}










