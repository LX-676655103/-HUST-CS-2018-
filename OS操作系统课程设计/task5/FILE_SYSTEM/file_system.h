#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <string>
using namespace std;

#define DISC_NAME "DISE"
#define BLOCK_SIZE 1024
#define STORAGE_SIZE 10240

#define MAX_NAME_LENGTH 10
#define MAX_PSW_LENGTH 10

#define MAX_USR_NUM 32
#define MAX_FILE_NUM 32

#define USR_EXIST 1
#define USR_NOT_EXIST 3
#define USR_FULL 2
#define USR_CREATE_SUCCESS 0
#define USR_CREATE_FAIL   -1

#define MAX_USR_NUM 32

#define FILE_TYPE 0
#define DIR_TYPE  1

#define FILE_EXIST 4
#define FILE_CREATE_SUCCESS 5
#define FILE_CREATE_FAILED  6
#define FILE_NOT_EXIST  7
#define DIR_EXIST 0
#define DIR_CREATE_SUCCESS 1
#define DIR_CREATE_FAILED  2
#define DIR_NOT_EXIST  3
#define CD_OP 0
#define MKDIR_OP 1
#define TOUCH_OP 2
#define CAT_OP 3


/* 磁盘块类(1024B) */
struct Block{
    char buffer[BLOCK_SIZE];
};

/* 磁盘*/
struct Storage{
    Block block[STORAGE_SIZE];
};
struct Usr {
    char usrname[MAX_NAME_LENGTH];
    char passname[MAX_PSW_LENGTH];
    /* root_block指向根目录块 */
    int  root_size;
    int  vaild;
    int  root_block;
};
struct File {
    char name[MAX_NAME_LENGTH];
    int  file_size;
    int  file_type;
    /* first_block指向文件第一个块 */
    int  first_block;
};
class File_system
{
private:
    int use_num;
    FILE* fdisc;
    /* 目录管理 */
    File* rootDir;     /* 根目录 */
    File* currentDir;  /* 当前目录 */
    int bk_fhead;  /* 空白块头指针 */
    int bk_nfree;  /* 空白块数 */

public:
    Storage storage;
    string dir_path;
    File_system();
    ~File_system();

    /* 文件块读取操作 */
    void read_from_Disc(int block, char* buf);
    void write_to_Disc(int block, char* buf);
    /* 用户操作 */
    int log_in(string &user, string &pass);
    int log_out();
    int Register(string &user, string &pass);
    int Remove(string &user, string &pass);
    int show_Usr();

    /* 文件操作 */
    void touch(string &filepath);
    int create_file(string &filename, int &filenum); /* 根目*/
    int delete_file(string &filepath); /* 根目*/
    int copy_file(string &frompath, string &topath);   /* 根目*/

    int open_file(string &filename);
    int catch_file(string &filepath); /* 根目*/
    int gedit_file(string &filepath); /* 根目*/

    /* 目录操作 */
    void mkdir(string &filepath);      /* mkdir命令 */
    void rmdir(string &filepath);      /* rmdir命令 */
    int create_dir(string &filename, int &filenum); /* 创建单级目录 */

    int delete_dir(string &filename, int &filenum); /* 删除目录 */
    int delete_dir_dfs(int block, int file_num); /* 删除目录 */
    int catch_dir();  /* 显示目录内容 */
    int show_path(string &path);  /* 显示路径 */

    /* 其他操作 */
    void cd_dir(string &filepath);
    void show_help();
    int get_blank_block();
    int get_blank_block1();

};

#endif
