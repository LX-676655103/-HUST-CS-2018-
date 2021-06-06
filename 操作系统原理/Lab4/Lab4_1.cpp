#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#include <ctime>
#include <pwd.h>
#include <grp.h>

void printdir(const char* path,char *dir, int depth);
void print_message(struct dirent *entry,struct stat &statbuf);

int main(int argc, char *argv[])
{
    struct stat statbuf;
    for(int i=1;i<argc;i++)
        printdir(argv[i],argv[i],1);
    return 0;
}

void printdir(const char* path,char *dir,int depth)
{
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    /*打开dir路径*/
    printf("%s:\n",path);
    printf("depth: %d\n",depth);
    if((dp=opendir(dir))==NULL)
    {
        printf("Failed to open directory!\n");
        return;
    }
    /*将dir设置为当前目录，并读下一个目录项*/
    chdir(dir);
    while(entry=readdir(dp))
    {
        lstat(entry->d_name,&statbuf);
        /*过滤'.'开头的文件和目录*/
        if(entry->d_name[0]=='.') continue;
        /*如果为目录文件*/
        if((statbuf.st_mode&S_IFMT)==S_IFDIR)
        {
            if(!strcmp(entry->d_name,".")||!strcmp(entry->d_name,".."))
                continue;
            print_message(entry,statbuf);
            printf("\n");
            char temp_path[1000];
            strcpy(temp_path,path);
            if(depth!=1)strcat(temp_path,"/");
            strcat(temp_path,entry->d_name);
            printdir(temp_path,entry->d_name,depth+1);
            printf("%s:\n",path);
            printf("depth: %d\n",depth);
            printf("\n");
        }
        else print_message(entry,statbuf);
    }
    /*返回父目录*/
    chdir("..");
    /*关闭目录项*/
    closedir(dp);
}
void print_message(struct dirent *entry,struct stat &statbuf)
{
    char filemode[11];
    switch (statbuf.st_mode&S_IFMT)
    {
        case S_IFREG: filemode[0]='-'; break;  /*普通文件*/
        case S_IFSOCK: filemode[0]='s'; break; /*套接字*/
        case S_IFLNK: filemode[0]='l'; break;  /*符号链接*/
        case S_IFBLK: filemode[0]='b'; break;  /*块设备*/
        case S_IFDIR: filemode[0]='d'; break;  /*目录文件*/
        case S_IFCHR: filemode[0]='c'; break;  /*字符设备*/
        case S_IFIFO: filemode[0]='p'; break;  /*管道*/
        default: break;
    }
    for(int i=1;i<=9;i++)
    {
        if(statbuf.st_mode&(1<<(9-i)))
            switch(i%3){
                case 0:filemode[i]='x'; break;
                case 1:filemode[i]='r'; break;
                case 2:filemode[i]='w'; break;
            }
        else filemode[i]='-';
    }
    filemode[10] = '\0';
    printf("%s ",filemode);
    printf("%5ld ",statbuf.st_nlink);
    printf("%s ",getpwuid(statbuf.st_uid)->pw_name);
    printf("%s ",getgrgid(statbuf.st_gid)->gr_name);
    printf("%9ld ",statbuf.st_size);
    printf(" %.12s ",4+ctime(&statbuf.st_mtime));
    printf("%s\n",entry->d_name);
}






