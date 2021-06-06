#include "main.h"

int main(int argc, char *argv[])
{
    int Sin=0;    /*环形缓冲队列键值*/
    int sem_empty,sem_full; /*信号灯*/

    /*获取环形缓冲队列的头键值与信号灯键值*/
    FILE *File_key=fopen("./key", "rb");
    if(File_key==NULL){
        printf("无法打开文件key!\n");
        exit(0);
    }
    fscanf(File_key,"%d %d %d",&Sin,&sem_empty,&sem_full);
    fclose(File_key);
    printf("%d %d %d\n",Sin,sem_empty,sem_full);

    /*打开源文件*/
    FILE *File_in=fopen(argv[0],"rb");

    /*循环获取数据*/
    int BUF_len,oper_num=0;
    ShareBuffer* pin;
    char temp[BUFLEN];
    while(TRUE)
    {
        P(sem_empty,0);
        /*一个进程对同一个共享内存多次shmat挂载，返回的指针不同且引用增加*/
        pin=(ShareBuffer*)shmat(Sin,NULL,0);

        memset(temp,0,BUFLEN);
        BUF_len=fread(temp,sizeof(char),BUFLEN,File_in);
        /*判断是否已经到文件尾*/
        if(!BUF_len){
            pin->status=STATUS_END;
            V(sem_full,0);
            fclose(File_in);
            exit(0);
        }
        /*printf("read: %s\n",temp);*/
        printf("No.%d Get->ShareBuffer ID: %d\n",++oper_num,Sin);
        memcpy(pin->data,temp,BUF_len);
        pin->length=BUF_len;
        pin->status=STATUS_FULL;
        Sin=pin->next_id;
        /*使用shmdt断开连接，防止虚拟内存溢出造成分配失败*/
        shmdt(pin);
        V(sem_full,0);
    }
}
