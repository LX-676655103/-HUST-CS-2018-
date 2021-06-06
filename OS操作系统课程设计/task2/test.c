#include<stdio.h>
#include<unistd.h>
#include<errno.h>
#include<sys/syscall.h>
int main(int argc,char **argv)
{
    /* ÊäÈë²ÎÊı´íÎó */
    if(argc!=3)
    {
        fprintf(stderr,"Usage:%s fromfile tofile\n\a",argv[0]);
        return(-1);
    }
    if(syscall(334, argv[1], argv[2])==-1)
        printf("ERROR\n");
    return 0;
}
