#include "main.h"

int main(int argc, char *argv[])
{
    int Sout=0;
    int sem_empty,sem_full;

    /*获取环形缓冲队列的头键值与信号灯键值*/
    FILE *File_key=fopen("./key", "rb");
    if(File_key==NULL){
        printf("无法打开文件key!\n");
        exit(0);
    }
    fscanf(File_key,"%d %d %d",&Sout,&sem_empty,&sem_full);
    fclose(File_key);
    printf("%d %d %d\n",Sout,sem_empty,sem_full);

    /*打开源文件*/
    FILE *File_out=File_out=fopen(argv[0], "wb");

    /*循环输出*/
    int oper_num=0;
    ShareBuffer* pout;
    char temp[BUFLEN];
    while(TRUE)
    {
        P(sem_full,0);
        /*将共享存储区附接到进程的虚拟地址空间*/
        pout=(ShareBuffer*)shmat(Sout,NULL,0);

        /*判断是否已经到文件尾*/
        if(pout->status==STATUS_END){
            fclose(File_out);
            exit(0);
        }
        /*从缓冲区读出数据并写入文件中*/
        memset(temp,0,BUFLEN);
        memcpy(temp,pout->data,pout->length);
        /*printf("write: %s\n",temp);*/
        printf("No.%d Put->ShareBuffer ID: %d\n",++oper_num,Sout);
        fwrite(temp,sizeof(char),pout->length,File_out);

        /*指向下一个共享缓冲区，并释放当前虚拟地址防止溢出*/
        Sout=pout->next_id;
        shmdt(pout);
        V(sem_empty,0);
    }
}



