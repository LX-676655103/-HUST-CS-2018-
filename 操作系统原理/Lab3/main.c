#include "main.h"
int Stail,Shead=0;      /*分别指向共享缓冲区链表的头和尾*/
int sem_empty,sem_full; /*信号灯分别指示是否为空和满*/
ShareBuffer* temp;      /*缓冲区临时操作指针*/
void delete_sem();
void delete_sbuf();
int main()
{
    /*创建键值为KEY的共享内存组，BUFLEN为缓冲区长度*/
    for(int i=0;i<BUFNUM;i++)
    {
        /*调用ftok函数生成键值*/
        key_t shmkey=ftok("./KEY",i);
        if(shmkey==ERROR){
            printf("无法获取key值!\n");
            exit(0);
        }
        /*调用shmget函数获取缓冲区句柄*/
        int Shm_id=shmget(shmkey,sizeof(ShareBuffer),IPC_CREAT|0666);
        if(Shm_id==ERROR){
            delete_sbuf();
            printf("无法创建共享缓冲区!\n");
            exit(0);
        }
        /*调用shmat函数获取缓冲区地址*/
        temp=(ShareBuffer*)shmat(Shm_id,NULL,0);
        if(Shead==0) Stail=Shm_id;
        temp->next_id=Shead;
        Shead=Shm_id;
    }
    /*将尾部指向头部，形成环形缓冲区*/
    temp=(ShareBuffer*)shmat(Stail,NULL,0);
    temp->next_id=Shead;

    /*创建信号灯*/
    sem_empty=semget(IPC_PRIVATE,1,IPC_CREAT|0666);
    sem_full=semget(IPC_PRIVATE,1,IPC_CREAT|0666);
    if(sem_full==-1||sem_empty==-1)
    {
        printf("信号灯创建失败!\n");
        delete_sem();
        delete_sbuf();
        exit(0);
    }

    /*信号灯赋初值*/
    arg.val=BUFNUM;
    if(semctl(sem_empty,0,SETVAL,arg)==-1){
        delete_sem();
        delete_sbuf();
        printf("信号灯初始化失败!\n");
        exit(0);
    }
    arg.val=0;
    if(semctl(sem_full,0,SETVAL,arg)==-1){
        delete_sem();
        delete_sbuf();
        printf("信号灯初始化失败!\n");
        exit(0);
    }

    /*传递键值打开并检查文件*/
    FILE *File_key=fopen("./key", "wb");
    if(File_key==NULL){
        delete_sem();
        delete_sbuf();
        printf("无法创建文件key!\n");
        exit(0);
    }
    fprintf(File_key,"%d %d %d",Shead,sem_empty,sem_full);
    fclose(File_key);

    /*打开并检查文件*/
    char path_in[100];
    FILE *File=NULL;
    while(!File){
        printf("请输入原文件地址：");
        scanf("%s",path_in);
        File=fopen(path_in, "rb");
    }
    printf("文件打开成功！\n");
    fclose(File);

    char path_out[100];
    File=NULL;
    while(!File){
        printf("请输入目的文件地址：");
        scanf("%s",path_out);
        File=fopen(path_out, "wb");
    }
    printf("文件打开成功！\n\n");
    fclose(File);

    /*创建并启动子进程GET、PUT*/
    char* argv_in[]={path_in,NULL};
    char* argv_out[]={path_out,NULL};
    pid_t Get_pro=fork();
    if(Get_pro==0)
        execv("./get",argv_in);
    pid_t Put_pro=fork();
    if(Put_pro==0)
        execv("./put",argv_out);

    /*等待两个子进程运行结束*/
    waitpid(Get_pro,NULL,0);
    waitpid(Put_pro,NULL,0);
    printf("子进程结束!\n");

    /*删除信号灯*/
    delete_sem();
    /*删除共享内存组*/
    delete_sbuf();
    exit(0);
}
void delete_sem()
{
    /*删除信号灯*/
    if(sem_empty!=-1)semctl(sem_empty,0,IPC_RMID,arg);
    if(sem_full!=-1)semctl(sem_full,0,IPC_RMID,arg);
    printf("信号灯删除成功!\n");
}
void delete_sbuf()
{
    int temp_id;
    /*删除共享内存组*/
    for(int i=0;i<BUFNUM;i++)
    {
        if(!Shead) break;
        /*printf("No.%d id: %d delete!\n",i+1,Shead);*/
        temp=(ShareBuffer*)shmat(Shead,NULL,0);
        temp_id=temp->next_id;
        shmctl(Shead,IPC_RMID,0);
        shmdt(temp);
        Shead=temp_id;
    }
    printf("共享内存删除成功!\n");
}







