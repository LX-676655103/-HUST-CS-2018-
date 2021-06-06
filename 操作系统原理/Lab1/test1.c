#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int pipefd[2];
int quitPid;
pid_t s1,s2;
int flag=1;
void my_func(int num)
{
    signal(SIGINT,SIG_DFL);
    kill(s1,SIGTERM);
    kill(s2,SIGTERM);
    flag=0;
}
void kill_s1(int num)
{
    printf("Child Process 1 is Killed by Parent!\n");
    exit(0);
}
void kill_s2(int num)
{
    printf("Child Process 2 is Killed by Parent!\n");
    exit(0);
}

int main() {
    signal(SIGINT,my_func);//父进程控制进程终止
    char buffer[128];//管道缓冲区
    pipe(pipefd);//管道初始化

    s1 = fork();//子进程s1创建
    if(s1 == 0){//s1子进程运行
        signal(SIGINT,SIG_IGN);
        signal(SIGTERM,kill_s1);
	while(1) {
		//lockf(pipefd[0],1,0);
                read(pipefd[0], buffer, 128);
                printf("s1 %s\n",buffer);
		//lockf(pipefd[0],0,0);
    }
    }
    else {//父进程执行
        s2 = fork();//s2子进程创建
        if(s2 == 0){//s2子进程

            signal(SIGINT,SIG_IGN);//忽略ctrl+c
            signal(SIGTERM,kill_s2);

            while(1) {
		//lockf(pipefd[0],1,0);
                read(pipefd[0], buffer, 128);
                printf("s2 %s\n",buffer);
		//lockf(pipefd[0],0,0);
            }
        }

        else {
            int x = 1;
            while(flag)
	    {
	    sprintf(buffer, "I send you %d times\n", x);
            write(pipefd[1] , buffer , 128);
            sleep(1);
            x++;
            if(x==6)
                my_func(1);
            }
            waitpid(s1,NULL,0);
            waitpid(s2,NULL,0);
            close(pipefd[0]);
            close(pipefd[1]);
            printf("Parent Process is Killed!\n");
	    printf("send %d times message!\n",--x);
            exit(0);
        }
    }

}


