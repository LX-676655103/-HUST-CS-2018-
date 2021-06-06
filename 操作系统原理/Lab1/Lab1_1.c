#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUF_LEN 100
#define PIPERRO -1
#define MSG_NUM 10

/*声明状态变量&消息总数计数器&管道端口*/
int status=1,pipefd[2];
int msg_count=0;

/*声明子程序号变量*/
pid_t Child_pro1,Child_pro2;

/*定义软中断处理函数*/
void my_sigint(int parameter);
void kill_child1(int parameter);
void kill_child2(int parameter);

int main()
{
    /*创建无名管道&数据缓冲区*/
    char buf[BUF_LEN];
    if(pipe(pipefd)==PIPERRO)
        printf("Can't create PIPE.\n");

    /*设置软中断信号SIGINT*/
    if(signal(SIGINT,my_sigint)==SIG_ERR)
        printf("Can't catch SIGINT.\n");

    /*创建子程序1*/
    Child_pro1=fork();
    if(Child_pro1==0)
    {
        /*忽略软中断信号SIGINT*/
        signal(SIGINT,SIG_IGN);
        /*设置软中断信号SIGUSR1*/
        if(signal(SIGUSR1,kill_child1)==SIG_ERR)
            printf("Can't catch SIGUSR1.\n");

        /*接收并显示数据*/
        msg_count=0;       //消息计数器清零
        close(pipefd[1]);  //关闭子进程2的写端
        while(status)
        {
            if(read(pipefd[0],buf,BUF_LEN))
                printf("Child Process 1: %s",buf);
            else break;
            msg_count++;
        }
        /*关闭管道*/
        close(pipefd[0]);  //关闭子进程1的读端

        /*打印信息并退出*/
        printf("\nChild Process 1 exit!\n");
        printf("Child Process 1 receive %d times message!\n",msg_count);
        exit(0);
    }

    /*创建子程序2*/
    Child_pro2=fork();
    if(Child_pro2==0)
    {
        /*忽略软中断信号SIGINT*/
        signal(SIGINT,SIG_IGN);
        /*设置软中断信号SIGUSR2*/
        if(signal(SIGUSR2,kill_child2)==SIG_ERR)
            printf("Can't catch SIGUSR2.\n");

        /*接收并显示数据*/
        msg_count=0;       //消息计数器清零
        close(pipefd[1]);  //关闭子进程2的写端
        while(status)
        {
            if(read(pipefd[0],buf,BUF_LEN))
                printf("Child Process 2: %s",buf);
            else break;
            msg_count++;
        }
        /*关闭管道*/
        close(pipefd[0]);  //关闭子进程2的读端

        /*打印信息并退出*/
        printf("\nChild Process 2 exit!\n");
        printf("Child Process 2 receive %d times message!\n",msg_count);
        exit(0);
    }

    /*定时发送数据*/
    msg_count=0;
    while(status)
    {
        msg_count++;
        sprintf(buf,"I send you %d times\n",msg_count);
        write(pipefd[1],buf,BUF_LEN);
        sleep(1);
        if(msg_count==MSG_NUM)
            break;
    }

    /*关闭管道*/
    close(pipefd[0]);      //关闭父进程的读端
    close(pipefd[1]);      //关闭父进程的写端


    /*等待子进程1&2退出*/
    waitpid(Child_pro1,NULL,0);
    waitpid(Child_pro2,NULL,0);


    /*打印信息并退出*/
    printf("\nParent Process exit!\n");
    printf("Parent Process send %d times message!\n",msg_count);
    exit(0);
}
void my_sigint(int parameter)
{
    signal(SIGINT,SIG_DFL);
    /*发送信号SIGUSR1给子进程1*/
    kill(Child_pro1,SIGUSR1);
    /*发送信号SIGUSR2给子进程2*/
    kill(Child_pro2,SIGUSR2);

    /*标记状态*/
    status=0;

    /*关闭管道*/
    close(pipefd[0]);      //关闭父进程的读端
    close(pipefd[1]);      //关闭父进程的读端

    /*等待子进程1&2退出*/
    waitpid(Child_pro1,NULL,0);
    waitpid(Child_pro2,NULL,0);

    /*打印信息并退出*/
    printf("\nParent Process is Killed!\n");
    printf("Parent Process send %d times message!\n",msg_count);
    exit(0);
}
void kill_child1(int parameter)
{
    /*标记状态*/
    status=0;
    /*关闭管道*/
    close(pipefd[0]);  //关闭子进程1的读端

    /*打印信息并退出*/
    printf("\nChild Process 1 is Killed by Parent!\n");
    printf("Child Process 1 receive %d times message!\n",msg_count);
    exit(0);
}
void kill_child2(int parameter)
{

    /*标记状态*/
    status=0;
    /*关闭管道*/
    close(pipefd[0]);  //关闭子进程2的读端

    /*打印信息并退出*/
    printf("\nChild Process 2 is Killed by Parent!\n");
    printf("Child Process 2 receive %d times message!\n",msg_count);
    exit(0);
}





