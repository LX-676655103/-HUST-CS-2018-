#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#define BUFFER_SIZE 1024

int main(int argc,char **argv)
{
    int from_fd,to_fd;
    int bytes_read,bytes_write;
    char buffer[BUFFER_SIZE];
    char *ptr;

    /* 输入参数错误 */
    if(argc!=3)
    {
        fprintf(stderr,"Usage:%s fromfile tofile\n\a",argv[0]);
        return(-1);
    }

    /* 打开源文件 */
    if((from_fd=open(argv[1],O_RDONLY))==-1)
    {
        fprintf(stderr,"Open %s Error:%s\n",argv[1],strerror(errno));
        return(-1);
    }

    /* 创建目的文件 */
    if((to_fd=open(argv[2],O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR))==-1)
    {
        fprintf(stderr,"Open %s Error:%s\n",argv[2],strerror(errno));
        return(-1);
    }

    /* 拷贝文件 */
    while(bytes_read=read(from_fd,buffer,BUFFER_SIZE))
    {
        /* 一个致命的错误发生了 */
        if((bytes_read==-1)&&(errno!=EINTR)) break;
        else if(bytes_read>0)
        {
            ptr=buffer;
            while(bytes_write=write(to_fd,ptr,bytes_read))
            {
                /* 一个致命错误发生了 */
                if((bytes_write==-1)&&(errno!=EINTR))break;
                /* 写完了所有读的字节 */
                else if(bytes_write==bytes_read) break;
                /* 只写了一部分,继续写 */
                else if(bytes_write>0)
                {
                    ptr+=bytes_write;
                    bytes_read-=bytes_write;
                }
            }
            /* 写的时候发生的致命错误 */
            if(bytes_write==-1)break;
        }
    }

    close(from_fd);
    close(to_fd);

    /* 读写错误 */
    if(bytes_read==-1||bytes_write==-1)
    {
        fprintf(stderr,"R/W ERROR:%s \n",strerror(errno));
        return(-1);
    }

    return(1);
}
