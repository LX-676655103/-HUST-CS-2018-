#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <string>

#define DISC_NAME          "DISE"
#define BLOCK_SIZE          1024
#define STORAGE_SIZE        10240

#define MAX_NAME_LENGTH     10
#define MAX_PSW_LENGTH      10

#define MAX_USR_NUM         32
#define MAX_FILE_NUM        32
#define MAX_USR_NUM         32

#define USR_EXIST           1
#define USR_NOT_EXIST       3
#define USR_FULL            2
#define USR_CREATE_SUCCESS  0
#define USR_CREATE_FAIL    -1

#define FILE_TYPE           0
#define DIR_TYPE            1

#define READ_WRITE          1
#define CREATE              2
#define SEARCH              3

using namespace std;

/* 磁盘块类(1024B) */
struct Block{
    char buffer[BLOCK_SIZE];
};
struct Usr {
    char usrname[MAX_NAME_LENGTH];
    char passname[MAX_PSW_LENGTH];
    int  root_size;  /* 根目录的大小 */
    int  vaild;      /* 有效性 */
    int  root_block; /* root_block指向根目录块 */
};
struct File_control_block {
    int block_num;
    int file_index;
};


struct File {
    char name[MAX_NAME_LENGTH*2];
    int  file_size;
    int  file_type;
    int  first_block; /* first_block指向文件第一个块 */
};


class File_system
{
private:
    int use_num;       /* 当前用户编号 */
    FILE* fdisc;       /* 磁盘文件指针 */

    /* 目录管理 */
    File* rootDir;     /* 根目录 */
    File* currentDir;  /* 当前目录 */
    File* tempDir;     /* 临时目录-用于路径解析 */

    Block zero_block;  /* 0号引导块 */
    Block temp_block;  /* 临时块 */

    int currentDir_num;   /* 当前目录块号 */
    int currentDir_index; /* 当前目录编号 */
    int tempDir_num;      /* 临时目录块号 */
    int tempDir_index;    /* 临时目录编号 */

    /* 文件控制块 */
    File_control_block file_control;
    File* currentFile;

    /* 空白块管理 */
    int bk_fhead;        /* 空白块头指针 */
    int bk_nfree;        /* 空白块数 */

public:
    string usr_name;
    string dir_path;

    File_system();
    ~File_system();

    /* 文件系统基本操作 */
    void read_from_Disc(int block, char* buf);  /* 从磁盘读取数据 */
    void write_to_Disc(int block, char* buf);   /* 向磁盘写入数据 */
    int get_block();                   /* 获取空白块 */
    int release_block(int block_num,int File_type);  /* 释放数据块 */
    int set_File_Pointer(File**file, int block_num, int index, Block &block); /* 设置文件指针 */
    int get_zero_block();
    int update_zero_block();

    /* 文件操作 */
    File_control_block* open_file(string &filepath, int mode);
    int read_from_file(File_control_block* file, int size, char *buf, int pos);
    int write_to_file(File_control_block* file, int size, const char* buf, int pos);
    int clear_file(File_control_block* file, int mode);
    File_control_block* close_file(File_control_block* file);

    /* 目录操作 */
    int create_dir(string &filename, int mode);
    int remove_dir(string &filename);
    int delete_dir_dfs(int block, int file_size);
    int list_files();
    int cd_dir(string &filepath);

    /* 用户操作 */
    int log_in(string &user, string &pass);
    int log_out();
    int Register(string &user, string &pass);
    int Remove(string &user, string &pass);
    int show_Usr();

    /* 拓展操作 */
    void touch(string &filepath);
    void rm(string &filepath);
    void cat(string &filepath);
    void gedit(string &filepath);
    void mkdir(string &filepath);
    void rmdir(string &filepath);
    void show_help();
};

File_system::~File_system()
{
    fclose(fdisc);
}

File_system::File_system()
{
    /* 创建磁盘文件
     * 0号块为引导块 */
    fdisc = nullptr;
    if(!(fdisc = fopen(DISC_NAME,"r+"))){
        fdisc = fopen(DISC_NAME,"a+");
        /* 如果磁盘文件未分配空间 */
        char buf[BLOCK_SIZE];
        memset(buf, 0, BLOCK_SIZE);
        /* 文件初始化与空白块成链 */
        for(int i=0;i<STORAGE_SIZE;i++){
            if(i==STORAGE_SIZE-1) *((int*)buf) = 0;
            else *((int*)buf) = i+1;
            fwrite(buf, sizeof(char), BLOCK_SIZE, fdisc);
        }
        fclose(fdisc);
        fdisc = fopen(DISC_NAME,"r+");
        /* 设置空白块的块数 */
        fread(buf, sizeof(char), BLOCK_SIZE, fdisc);
        ((int*)buf)[1] = STORAGE_SIZE - 1;
        fseek(fdisc, 0, SEEK_SET);
        fwrite(buf, sizeof(char), BLOCK_SIZE, fdisc);
    }
    fseek(fdisc, 0, SEEK_SET);

}

/* 文件系统基本操作 */

/* 从磁盘读取数据
 * block: 磁盘块号
 * buf: 磁盘块数据缓冲区
 */
void File_system::read_from_Disc(int block, char* buf)
{
    fseek(fdisc, block*BLOCK_SIZE, SEEK_SET);
    fread(buf, sizeof(char), BLOCK_SIZE, fdisc);
}

/* 向磁盘写入数据
 * block: 磁盘块号
 * buf: 磁盘块数据缓冲区
 */
void File_system::write_to_Disc(int block, char* buf)
{
    fseek(fdisc, block*BLOCK_SIZE, SEEK_SET);
    fwrite(buf, sizeof(char), BLOCK_SIZE, fdisc);
}

/* 获取空白块
 * 返回值int: 正常返回可用的空白块号，异常返回0
 */
int File_system::get_block()
{
    int temp_bk=bk_fhead;
    if(bk_nfree==0) return 0;
    else
    {
        char temp_buf[BLOCK_SIZE];
        read_from_Disc(temp_bk, temp_buf);
        bk_fhead = *((int*)temp_buf);
        bk_nfree--;
        return temp_bk;
    }
}

/* 释放数据块
 * block_num: 文件的首个磁盘块号
 * File_type: 文件类型
 */
int File_system::release_block(int block_num,int File_type)
{
    Block block;
    if(File_type==DIR_TYPE)
    {
        read_from_Disc(block_num, block.buffer);
        *((int*)block.buffer)=bk_fhead;
        write_to_Disc(block_num, block.buffer);
        bk_fhead=block_num;
        bk_nfree++;
        return 0;
    }
    while(block_num!=0)
    {
        read_from_Disc(block_num, block.buffer);
        int tempnum=*((int*)block.buffer);
        *((int*)block.buffer)=bk_fhead;
        write_to_Disc(block_num, block.buffer);
        bk_fhead=block_num;
        block_num=tempnum;
        bk_nfree++;
    }
    return 0;
}

/* 设置文件指针
 * file: 文件双重指针
 * block_num: 文件块号
 * index: 文件索引
 * block: 缓冲区
 */
int File_system::set_File_Pointer(File**file, int block_num, int index, Block &block)
{
    /* 从磁盘中加载当前文件目录块 */
    read_from_Disc(block_num, block.buffer);
    if(!block_num)    /* 当前块为0号引导块 */
        *file = &((File*)(&block.buffer[8]))[index];
    else *file = &((File*)block.buffer)[index];
    return 0;
}

/* 获取0号引导块
 * 将0号引导块加载到内存
 */
int File_system::get_zero_block()
{
    read_from_Disc(0, zero_block.buffer);
    return 0;
}

/* 更新0号引导块
 * 更新空白块情况
 */
int File_system::update_zero_block()
{
    char buf[BLOCK_SIZE];
    /* 更新引导块 */
    fseek(fdisc, 0, SEEK_SET);
    read_from_Disc(0, buf);
    *((int*)buf) = bk_fhead;   /* 空白块头指针 */
    ((int*)buf)[1] = bk_nfree;  /* 空白块数 */
    write_to_Disc(0, buf);
    return  0;
}



/* 文件操作 */

/* 打开文件函数
 * filepath: 文件路径，仅支持相对路径
 * mode: 打开方式控制变量
 *     READ_WRITE: 对文件进行读写，文件不存在时返回异常
 *     CREATE: 创建文件（可读写），不存在时创建文件与目录
 * 正常返回值int: 若当前文件存在, 或创建成功则返回对应的编号
 * 异常返回int: block_num为-1表示目录名与文件名冲突，
 *                       为-2表示当前空白块耗尽，为-3表示目录已满
 */
File_control_block* File_system::open_file(string &filepath, int mode)
{
    /* 设置临时目录为当前目录 */
    tempDir_num=currentDir_num;
    tempDir_index=currentDir_index;
    set_File_Pointer(&tempDir, tempDir_num, tempDir_index, temp_block);

    while (true)
    {
        /* 如果开头为/ */
        if(filepath[0]=='/')
        {
            /* 设置临时目录为根目录 */
            tempDir_num=0;
            tempDir_index=use_num;
            set_File_Pointer(&tempDir, tempDir_num, tempDir_index, temp_block);
            filepath = filepath.substr(1);
        }
        else if(filepath[0]=='.'&&filepath[1]=='/') /* 如果开头为./ */
            filepath = filepath.substr(2);

        /* 如果无下级目录 */
        if(filepath.find('/')==filepath.npos)
        {
            /* filelist指向文件目录项 */
            Block filelist_block;
            read_from_Disc(tempDir->first_block, filelist_block.buffer);
            File* filelist = (File*)filelist_block.buffer;
            int file_num = tempDir->file_size/32;

            /* 查询是否已经存在该文件 */
            for(int i=0;i<file_num;i++)
            {
                if(!strcmp(filelist[i].name, filepath.c_str()))
                {
                    File_control_block* temp_control = new File_control_block;
                    if(filelist[i].file_type==FILE_TYPE)
                    {
                        /* 返回文件控制块信息 */
                        temp_control->block_num=tempDir->first_block;
                        temp_control->file_index=i;
                        return temp_control;
                    }
                    else
                    {
                        /* 当前文件名称与目录冲突 */
                        temp_control->block_num=-1;
                        temp_control->file_index=0;
                        return temp_control;
                    }
                }
            }
            if(mode==READ_WRITE)
            {
                /* READ_WRITE状态下，当前文件不存在，返回异常 */
                File_control_block* temp_control = new File_control_block;
                temp_control->block_num=0;
                temp_control->file_index=0;
                return temp_control;
            }
            if(tempDir->file_size>=1024)
            {
                /* 当前目录已满 */
                File_control_block* temp_control = new File_control_block;
                temp_control->block_num=-3;
                temp_control->file_index=0;
                return temp_control;
            }

            /*  从磁盘申请空白块 */
            int newblock = get_block();
            if(!newblock)
            {
                /* 当前空间已满 */
                File_control_block* temp_control = new File_control_block;
                temp_control->block_num=-2;
                temp_control->file_index=0;
                return temp_control;
            }
            /* 修改文件头文件块的指针 */
            Block temp;
            read_from_Disc(newblock, temp.buffer);
            *((int*)temp.buffer)=0;
            write_to_Disc(newblock, temp.buffer);

            /* 修改并更新文件目录项 */
            tempDir->file_size += 32;
            write_to_Disc(tempDir_num, temp_block.buffer);

            /* 创建新文件并更新文件块 */
            File* newFile = &filelist[file_num];
            newFile->first_block=newblock;
            newFile->file_size = 0;
            newFile->file_type = FILE_TYPE;
            strcpy(newFile->name,filepath.c_str());
            write_to_Disc(tempDir->first_block, filelist_block.buffer);

            File_control_block* temp_control = new File_control_block;
            temp_control->block_num=tempDir->first_block;
            temp_control->file_index=file_num;
            return temp_control;
        }

        /* 路径切分解析 */
        string spilt=filepath.substr(0, filepath.find('/'));
        filepath=filepath.substr(filepath.find('/')+1);
        tempDir_index=create_dir(spilt, CREATE);
        if(tempDir_index < 0)
        {
            File_control_block* temp_control = new File_control_block;
            temp_control->block_num=-4;
            temp_control->file_index=0;
            return temp_control;
        }

        /* 更新当前文件目录块 */
        write_to_Disc(tempDir_num, temp_block.buffer);
        /* 加载下一级文件目录块 */
        tempDir_num = tempDir->first_block;
        set_File_Pointer(&tempDir, tempDir_num, tempDir_index, temp_block);
    }
}

/* 读取函数
 * file: 文件控制块指针，指向文件控制块
 * size: 读取的数据大小
 * buf: 缓冲区
 * pos: 读取数据的偏移
 * 返回值int: 正常返回读取数据的大小，异常返回-1
 */
int File_system::read_from_file(File_control_block* file, int size, char *buf, int pos)
{
    /* 当前文件无法读取 */
    if(file->block_num <= 0) return -1;
    if(pos < 0 || size < 0 || buf == nullptr) return -1;

    /* 读取数据到缓冲区 */
    Block block;
    read_from_Disc(file->block_num, block.buffer);
    File* pfile = &((File*)block.buffer)[file->file_index];

    int tempblock=pfile->first_block;
    int tempsize=pfile->file_size;
    read_from_Disc(tempblock, block.buffer);

    int index=0;
    int block_length=BLOCK_SIZE-4;
    while(tempsize!=0)
    {
        if(!size) return index;
        if(!block_length){
            block_length=BLOCK_SIZE-4;
            tempblock=*((int*)block.buffer);
            read_from_Disc(tempblock, block.buffer);
        }
        if(!pos){
            buf[index] = block.buffer[BLOCK_SIZE-block_length];
            index++;
            size--;
        }
        else pos--;
        tempsize--;
        block_length--;
    }
    return index;
}

/* 写入函数
 * file: 文件控制块指针，指向文件控制块
 * size: 读取的数据大小
 * buf: 缓冲区
 * pos: 读取数据的偏移, 超过文件大小默认为文件末尾
 * 返回值int: 正常返回读取数据的大小，异常返回-1
 */
int File_system::write_to_file(File_control_block* file, int size, const char* buf, int pos)
{
    /* 当前文件无法写入 */
    if(file->block_num <= 0) return -1;
    if(pos < 0 || size < 0 || buf == nullptr) return -1;

    /* 创建中间缓冲区，读取原数据 */
    Block block;
    read_from_Disc(file->block_num, block.buffer);
    File* pfile = &((File*)block.buffer)[file->file_index];
    int tempsize=pfile->file_size;
    if(pos>tempsize) pos=tempsize; /* 超过文件大小默认为文件末尾 */
    int buf_length = ((pos+size) > tempsize) ? (pos+size) : tempsize;
    char *tempbuf = new char[buf_length];

    read_from_file(file, pos, tempbuf, 0);
    for(int i=pos;i<pos+size;i++)
        tempbuf[i]=buf[i-pos];
    if((pos+size) < tempsize)
        read_from_file(file, tempsize, tempbuf+pos+size, pos+size);

    /* 申请空白块 */
    int tempblock=get_block();
    if(!tempblock)
    {
        delete [] tempbuf;
        return -1;
    }

    /* 加载磁盘块, 将当前块设置为头块  */
    read_from_Disc(tempblock, block.buffer);
    *((int*)block.buffer)=0;
    int head_block=tempblock;

    /* 刷新文件 */
    tempsize=buf_length;
    int block_length=BLOCK_SIZE-4;
    while(tempsize)
    {
        /* 原空白块已满，申请新空白块 */
        if(!block_length)
        {
            block_length=BLOCK_SIZE-4;
            int temp=get_block();
            /* 空白块申请失败 */
            if(!temp){
                *((int*)block.buffer)=0;
                write_to_Disc(tempblock, block.buffer);
                release_block(head_block, FILE_TYPE);
                delete [] tempbuf;
                return -1;
            }

            /* 更新磁盘块 */
            *((int*)block.buffer)=temp;
            write_to_Disc(tempblock, block.buffer);
            /* 加载磁盘块 */
            read_from_Disc(temp, block.buffer);
            tempblock=temp;
        }

        block.buffer[BLOCK_SIZE-block_length]=tempbuf[buf_length-tempsize];
        block_length--;
        tempsize--;
    }

    /* 更新磁盘块 */
    *((int*)block.buffer)=0;
    write_to_Disc(tempblock, block.buffer);

    read_from_Disc(file->block_num, block.buffer);
    pfile = &((File*)block.buffer)[file->file_index];

    /*释放原有的空白块*/
    release_block(pfile->first_block, FILE_TYPE);

    /* 更新文件目录块 */
    pfile->file_size=buf_length;
    pfile->first_block=head_block;
    write_to_Disc(file->block_num, block.buffer);
    delete [] tempbuf;
    return 0;

}

/* 文件关闭函数
 * file: 文件控制块指针，指向文件控制块
 * 返回值int: 正常返回nullptr，异常返回-1
 */
File_control_block* File_system::close_file(File_control_block* file)
{
    delete file;
    return nullptr;
}

/* 文件内容清空函数
 * file: 文件控制块指针，指向文件控制块
 * mode: mode为1时表示删除文件，不保留任何块；mode为2时表示清除内容，保留头块
 * 返回值int: 正常返回0，异常返回-1
 */
int File_system::clear_file(File_control_block* file, int mode)
{
    Block block;
    read_from_Disc(file->block_num, block.buffer);
    File* pfile = &((File*)block.buffer)[file->file_index];

    /*释放原有的空白块*/
    release_block(pfile->first_block, FILE_TYPE);

    if(mode==1) return 0;
    /*  从磁盘申请空白块 */
    int newblock = get_block();
    if(!newblock) return -1;
    /* 修改文件头文件块的指针 */
    Block temp;
    read_from_Disc(newblock, temp.buffer);
    *((int*)temp.buffer)=0;
    write_to_Disc(newblock, temp.buffer);

    /* 更新文件目录块 */
    pfile->file_size=0;
    pfile->first_block=newblock;
    write_to_Disc(file->block_num, block.buffer);
    return 0;
}


/* 目录操作 */

/* 目录创建函数, 使用之前需要设置tempDir
 * filename:  目录名
 * mode: 打开方式控制变量
 *     SEARCH: 查找当前目录，不存在时返回异常-1
 *     CREATE: 创建目录，不存在时创建目录
 * 正常返回值int: 若当前文件存在, 或创建成功则返回对应的目录项编号
 * 异常返回int: 返回-1表示目录名与文件名冲突，返回-2表示当前空白块耗尽，返回-3表示目录已满
 */
int File_system::create_dir(string &filename, int mode)
{
    int file_num = tempDir->file_size/32;
    /* filelist指向文件目录项 */
    Block filelist_block;
    read_from_Disc(tempDir->first_block, filelist_block.buffer);
    File* filelist = (File*)filelist_block.buffer;

    filelist[0].file_size = tempDir_index;
    filelist[0].first_block = tempDir_num;
    Block block; File* temp_file;
    set_File_Pointer(&temp_file, tempDir_num, 0, block);
    if(!tempDir_num)
    {
        filelist[1].file_size = tempDir_index;
        filelist[1].first_block = tempDir_num;
    }
    else {
        filelist[1].file_size = temp_file->file_size;
        filelist[1].first_block = temp_file->first_block;
    }

    write_to_Disc(tempDir->first_block, filelist_block.buffer);

    read_from_Disc(tempDir->first_block, filelist_block.buffer);
    filelist = (File*)filelist_block.buffer;

    /* 查询是否已经存在该目录 */
    for(int i=0;i<file_num;i++)
    {
        if(!strcmp(filelist[i].name, filename.c_str()))
        {
            if(filelist[i].file_type==DIR_TYPE) return i;
            else return -1;
        }
    }

    /* SEARCH状态下，当前目录不存在，返回异常 */
    if(mode==SEARCH) return -1;

    /* CREATE状态下，当前目录不存在
     * 当前目录不存在，需要创建当前目录
     * 若当前目录已满，则创建失败
     * 否则，将创建当前目录
     */
    if(tempDir->file_size>=1024) return -3;

    /*  从磁盘申请空白块 */
    int newblock = get_block();
    if(!newblock) return -2;

    /* 修改并更新文件目录项 */
    tempDir->file_size += 32;
    write_to_Disc(tempDir_num, temp_block.buffer);


    /* 创建新目录并更新目录块 */
    File* newDir = &filelist[file_num];
    newDir->file_type = DIR_TYPE;
    newDir->first_block = newblock;
    newDir->file_size = 64;
    strcpy(newDir->name,filename.c_str());
    write_to_Disc(tempDir->first_block, filelist_block.buffer);

    /*  修正当前目录的内容, 添加 .与..目录 */
    Block temp;
    read_from_Disc(newblock, temp.buffer);
    File* pdir = (File*)temp.buffer; /* 使用文件指针pdir进行文件操作 */

    strcpy(pdir[0].name,".");
    pdir[0].file_size = file_num;
    pdir[0].file_type = DIR_TYPE;
    pdir[0].first_block = tempDir->first_block;

    strcpy(pdir[1].name,"..");
    pdir[1].file_size = filelist[0].file_size;
    pdir[1].file_type = DIR_TYPE;
    pdir[1].first_block = filelist[0].first_block;
    write_to_Disc(newblock, temp.buffer);

    return file_num;
}

/* 目录移除函数, 使用之前需要设置tempDir
 * filename: 目录名
 * 返回值int: 返回0，删除成功；返回-1，目录不存在
 */
int File_system::remove_dir(string &filename)
{
    /* 使用create_dir函数查找当前目录是否存在 */
    int file_num = create_dir(filename, SEARCH);
    if(file_num < 0) return -1;

    /* 加载对应的文件目录块 */
    Block block; File* temp;
    int temp_num = tempDir_num;
    int temp_index = tempDir_index;
    set_File_Pointer(&temp, tempDir->first_block, file_num, block);

    /* 递归删除当前目录下的所有文件 */
    delete_dir_dfs(temp->first_block, temp->file_size);

    /* 加载对应的文件目录块 */
    tempDir_num=temp_num;
    tempDir_index=temp_index;
    set_File_Pointer(&tempDir, tempDir_num, tempDir_index, temp_block);

    /* 修改文件目录块 */
    tempDir->file_size-=32;
    write_to_Disc(tempDir_num, temp_block.buffer);

    read_from_Disc(tempDir->first_block, block.buffer);
    File* curr_filelist=(File*)block.buffer;
    while (file_num<tempDir->file_size/32)
    {
        curr_filelist[file_num]=curr_filelist[file_num+1];
        file_num++;
    }
    write_to_Disc(tempDir->first_block, block.buffer);
    return 0;
}

/* 目录递归删除函数，删除该目录下的所有子文件
 * block: 目录名
 * file_size: 目录文件的大小
 * 返回值int: 返回0，目录删除成功；返回-1，目录删除失败
 */
int File_system::delete_dir_dfs(int block, int file_size)
{
    /* filelist指向文件目录项 */
    Block File_block;
    read_from_Disc(block, File_block.buffer);
    File* filelist = (File*)File_block.buffer;

    /* 循环查询每个文件目录项 */
    int max_file_num = file_size/32;
    for(int i=2;i<max_file_num;i++)
    {
        /* 如果当前为文件，则释放所有空白块 */
        if(filelist[i].file_type==FILE_TYPE)
            release_block(filelist[i].first_block, FILE_TYPE);
        /* 如果当前为目录，则递归删除 */
        if(filelist[i].file_type==DIR_TYPE)
            delete_dir_dfs(filelist[i].first_block, filelist[i].file_size);
    }

    /* 释放当前目录块 */
    *((int*)File_block.buffer)=bk_fhead;
    bk_fhead=block;
    bk_nfree++;
    write_to_Disc(block, File_block.buffer);
    return 0;
}

/* 显示目录下文件函数
 * 显示当前目录下的所有文件
 */
int File_system::list_files()
{
    /* 加载当前目录 */
    set_File_Pointer(&currentDir, currentDir_num, currentDir_index, temp_block);

    Block tempblock;
    read_from_Disc(currentDir->first_block, tempblock.buffer);

    cout << usr_name << "/" << dir_path << ":" << endl;
    for(int i=2;i<currentDir->file_size/32;i++)
    {
        if(((File*)tempblock.buffer)[i].file_type==FILE_TYPE) cout << "FILE ";
        else cout << "DIR  ";
        cout << ((File*)tempblock.buffer)[i].name << "   ";
        cout << ((File*)tempblock.buffer)[i].file_size;
        cout <<" B  "<< endl;
    }
    return 0;
}

/* 切换当前目录函数
 * filepath: 切换到filepath指向的目录，支持绝对路径以及相对路径
 * 返回值int: 返回0成功, 返回-1失败
 */
int File_system::cd_dir(string &filepath)
{
    string path = dir_path;
    /* 从磁盘中加载当前文件目录块 */
    tempDir_num=currentDir_num;
    tempDir_index=currentDir_index;
    set_File_Pointer(&tempDir, tempDir_num, tempDir_index, temp_block);

    while (true) {
        /* 如果开头为/ */
        if(filepath[0]=='/')
        {
            /* 设置临时目录为根目录 */
            tempDir_num=0;
            tempDir_index=use_num;
            set_File_Pointer(&tempDir, tempDir_num, tempDir_index, temp_block);
            filepath = filepath.substr(1);
            if(filepath.length()==0)
            {
                currentDir_num=0;
                currentDir_index=use_num;
                set_File_Pointer(&currentDir, currentDir_num, currentDir_index, temp_block);
                dir_path = "";
                return 0;
            }
        }
        else if(filepath[0]=='.'&&filepath[1]=='/') /* 如果开头为./ */
            filepath = filepath.substr(2);
        else if(filepath[0]=='.'&&filepath[1]!='.')   /* 如果开头为.当级目录 */
        {
            filepath = filepath.substr(1);
            continue;
        }

        /* 如果无下级目录 */
        if(filepath.find('/')==filepath.npos)
        {
            /* 使用create_dir函数查找当前目录是否存在 */
            int file_num = create_dir(filepath, SEARCH);
            if(file_num < 0) return -1;

            /* 设置为当前目录 */
            currentDir_num=tempDir->first_block;
            currentDir_index=file_num;
            set_File_Pointer(&currentDir, currentDir_num, currentDir_index, temp_block);

            /* 如果当前..上级目录 */
            string tempstr = currentDir->name;
            if(tempstr=="..")
            {
                /* 如果需要切换的为根目录 */
                if(!currentDir->first_block)
                {
                    currentDir_num=0;
                    currentDir_index=use_num;
                    set_File_Pointer(&currentDir, currentDir_num, currentDir_index, temp_block);
                }
                else
                {
                    currentDir_index = currentDir->file_size;
                    currentDir_num = currentDir->first_block;
                    read_from_Disc(currentDir_num, temp_block.buffer);
                    currentDir = &((File*)temp_block.buffer)[currentDir_index];
                }

                /* 修正当前目录 */
                path=path.substr(0,path.length()-1);
                path=path.substr(0,path.find_last_of("/")+1);
            }
            else path += (tempstr + "/");

            dir_path = path;
            return 0;
        }

        /* 路径切分解析 */
        string spilt=filepath.substr(0, filepath.find('/'));
        filepath=filepath.substr(filepath.find('/')+1);
        tempDir_index=create_dir(spilt, SEARCH);
        if(tempDir_index < 0) return -1;

        /* 更新当前文件目录块 */
        write_to_Disc(tempDir_num, temp_block.buffer);
        /* 加载下一级文件目录块 */
        tempDir_num = tempDir->first_block;
        set_File_Pointer(&tempDir, tempDir_num, tempDir_index, temp_block);

        /* 如果当前..上级目录 */
        string tempstr = tempDir->name;
        if(tempstr=="..")
        {
            /* 如果需要切换的为根目录 */
            if(!tempDir->first_block)
            {
                tempDir_num=0;
                tempDir_index=use_num;
                set_File_Pointer(&tempDir, tempDir_num, tempDir_index, temp_block);
            }
            else
            {
                tempDir_index = tempDir->file_size;
                tempDir_num = tempDir->first_block;
                read_from_Disc(tempDir_num, temp_block.buffer);
                tempDir = &((File*)temp_block.buffer)[tempDir_index];
            }
            path=path.substr(0,path.length()-1);
            path=path.substr(0,path.find_last_of("/")+1);
        }
        else path += (tempstr + "/");
    }
}


/* 用户操作 */

/* 用户注册函数 */
int File_system::Register(string &user, string &pass)
{
    int usr_num = 0;
    char buf[BLOCK_SIZE];

    /* 读取引导块信息 */
    read_from_Disc(0, buf);
    Usr* currUsr = (Usr*)&buf[8];
    bk_fhead = *((int*)buf);
    bk_nfree = ((int*)buf)[1];

    /* 查找当前是否存在该用户 */
    for(;usr_num<MAX_USR_NUM;usr_num++)
    {
        if(!currUsr[usr_num].vaild) break;
        if(!strcmp(currUsr[usr_num].usrname, user.c_str()))
        {
            cout << "当前用户已存在！" << endl;
            return USR_EXIST;
        }
    }
    if(usr_num==MAX_USR_NUM) return USR_FULL;

    /* 创建用户 */
    int root = get_block();
    if(!root) return  USR_CREATE_FAIL;
    update_zero_block();

    read_from_Disc(0, buf);
    currUsr = (Usr*)&buf[8];

    currUsr[usr_num].root_block = root;
    currUsr[usr_num].root_size = 64;
    currUsr[usr_num].vaild=1;
    strcpy(currUsr[usr_num].usrname,user.c_str());
    strcpy(currUsr[usr_num].passname,pass.c_str());
    write_to_Disc(0, buf);

    /* 添加 .与..目录 */
    read_from_Disc(currUsr[usr_num].root_block, buf);
    File* pdir = (File*)buf; /* 使用文件指针pdir进行文件操作 */
    strcpy(pdir[0].name,".");
    pdir[0].file_size = usr_num;
    pdir[0].file_type = DIR_TYPE;
    pdir[0].first_block = 0;

    strcpy(pdir[1].name,"..");
    pdir[1].file_size = usr_num;
    pdir[1].file_type = DIR_TYPE;
    pdir[1].first_block = 0;

    /* 将修改结果写回 */
    write_to_Disc(root, buf);
    cout << "用户创建成功!" << endl;
    return USR_CREATE_SUCCESS;
}

/* 用户移除函数 */
int File_system::Remove(string &user, string &pass)
{
    int num = log_in(user, pass);
    if(num != USR_EXIST)
    {
        cout << "移除失败！" << endl;
        return -1;
    }
    delete_dir_dfs(rootDir->first_block, rootDir->file_size);
    log_out();

    char buf[BLOCK_SIZE];
    /* 读取引导块信息 */
    read_from_Disc(0, buf);
    Usr* currUsr = (Usr*)&buf[8];
    int i=use_num;
    for(;i<MAX_USR_NUM;i++)
    {
        if(!currUsr[i+1].vaild) break;
        currUsr[i] = currUsr[i+1];
    }
    currUsr[i].vaild = 0;
    write_to_Disc(0, buf);
    return 0;
}

/* 用户登录函数 */
int File_system::log_in(string &user, string &pass)
{

    int usr_num = 0;
    char buf[BLOCK_SIZE];
    /* 读取引导块信息 */
    read_from_Disc(0, buf);
    Usr* currUsr = (Usr*)&buf[8];
    /* 查找当前是否存在该用户 */
    for(;usr_num<MAX_USR_NUM;usr_num++)
    {
        if(!currUsr[usr_num].vaild) return USR_NOT_EXIST;
        if(!strcmp(currUsr[usr_num].usrname, user.c_str()))
        {
            cout << "用户名正确！" << endl;
            if(!strcmp(currUsr[usr_num].passname, pass.c_str()))
            {
                cout << "密码正确！" << endl;
                use_num = usr_num;
                /* 设置当前的根目录 读取引导块 */
                read_from_Disc(0, zero_block.buffer);
                currUsr = (Usr*)&zero_block.buffer[8];
                rootDir = (File*)(&currUsr[usr_num]);

                /* 设置当前目录以及临时目录的目录块 */
                currentDir=rootDir;
                currentDir_num=0;
                currentDir_index=usr_num;
                tempDir=rootDir;
                tempDir_num=0;
                tempDir_index=usr_num;

                /* 加载空白块处理逻辑 */
                read_from_Disc(0, buf);
                bk_fhead = *((int*)buf);  /* 空白块头指针 */
                bk_nfree = ((int*)buf)[1];  /* 空白块数 */
                usr_name = currUsr[usr_num].usrname;
                return USR_EXIST;
            }
        }
    }
    return USR_NOT_EXIST;
}

/* 用户登出函数 */
int File_system::log_out()
{
    /* 更新引导块 */
    update_zero_block();
    usr_name = "";
    dir_path = "";
    return 0;
}

/* 用户显示函数 */
int File_system::show_Usr()
{
    int usr_num = 0;
    char buf[BLOCK_SIZE];
    /* 读取引导块信息 */
    read_from_Disc(0, buf);

    cout << "free block num: "<< ((int*)buf)[1] << endl << endl;
    Usr* currUsr = (Usr*)&buf[8];
    /* 查找当前是否存在该用户 */
    for(;usr_num<MAX_USR_NUM;usr_num++)
    {
        if(!currUsr[usr_num].vaild) break;
        cout << "Name: "<< currUsr[usr_num].usrname << endl;
        cout << "root: "<< currUsr[usr_num].root_block << endl;
    }
    return 0;

}


/* 拓展操作 */

void File_system::touch(string &filepath)
{
    File_control_block* file = open_file(filepath, CREATE);
    if(file->block_num==-1) cout << "文件名与目录名冲突!" << endl;
    else if(file->block_num==-2) cout << "空白块耗尽!" << endl;
    else if(file->block_num==-3) cout << "目录已满!" << endl;
    else if(file->block_num==-4) cout << "路径目录创建失败!" << endl;
}

void File_system::rm(string &filepath)
{
    File_control_block* file = open_file(filepath, READ_WRITE);
    if(file->block_num<=0) cout << "当前文件不存在!" << endl;
    else {
        clear_file(file, 1);
        Block block;
        /* 加载对应的文件目录块 */
        read_from_Disc(file->block_num, block.buffer);
        File* filelist = (File*)block.buffer;

        /* 修改文件目录块 */
        tempDir_num=filelist[0].first_block;
        tempDir_index=filelist[0].file_size;

        set_File_Pointer(&tempDir, tempDir_num, tempDir_index, temp_block);
        tempDir->file_size-=32;
        write_to_Disc(tempDir_num, temp_block.buffer);

        int file_num = file->file_index;
        while (file_num<tempDir->file_size/32)
        {
            filelist[file_num]=filelist[file_num+1];
            file_num++;
        }
        write_to_Disc(file->block_num, block.buffer);
        cout << "文件删除成功!" << endl;
    }
}

void File_system::cat(string &filepath)
{
     File_control_block* file = open_file(filepath, READ_WRITE);
     if(file->block_num<=0) cout << "当前文件不存在!" << endl;
     else {
         Block block;
         read_from_Disc(file->block_num, block.buffer);
         File* pfile = &((File*)block.buffer)[file->file_index];

         int size = pfile->file_size;
         char *buf = new char[size];
         read_from_file(file, size, buf, 0);
         for(int i=0;i<size;i++)
             cout << buf[i];
         cout << endl;
         delete[] buf;
     }
}

void File_system::gedit(string &filepath)
{
    File_control_block* file = open_file(filepath, READ_WRITE);
    if(file->block_num<=0) cout << "当前文件不存在!" << endl;
    else {
        /* 显示原有文本内容 */
        cat(filepath);

        /* 读入新的文本 */
        int size = 0; char c;
        char *buf = nullptr;
        cout << "请输入需要输入的文本(\\n结尾):" << endl;
        while((c=getchar())!='\n')
        {
            if(!(size % BLOCK_SIZE))
            {
                if(!buf) buf = new char[BLOCK_SIZE];
                else {
                    int buf_num = size/BLOCK_SIZE;
                    char *tempbuf = new char[buf_num*BLOCK_SIZE];
                    for(int i=0;i<size;i++)
                        tempbuf[i] = buf[i];
                    delete [] buf;
                    buf = tempbuf;
                }
            }
            buf[size++] = c;
        }
        clear_file(file, 2);
        write_to_file(file, size, buf, 0);
    }
}

void File_system::mkdir(string &filepath)
{
    /* 从磁盘中加载当前文件目录块 */
    tempDir_num=currentDir_num;
    tempDir_index=currentDir_index;
    set_File_Pointer(&tempDir, tempDir_num, tempDir_index, temp_block);

    while (true) {
        /* 如果开头为/ */
        if(filepath[0]=='/')
        {
            /* 设置临时目录为根目录 */
            tempDir_num=0;
            tempDir_index=use_num;
            set_File_Pointer(&tempDir, tempDir_num, tempDir_index, temp_block);
            filepath = filepath.substr(1);
        }
        else if(filepath[0]=='.'&&filepath[1]=='/') /* 如果开头为./ */
            filepath = filepath.substr(2);

        /* 如果无下级目录 */
        if(filepath.find('/')==filepath.npos)
        {
            int file_num = create_dir(filepath, CREATE);
            if(file_num==-1) cout << "目录名与文件名冲突!" << endl;
            else if(file_num==-2) cout << "空白块耗尽!" << endl;
            else if(file_num==-3) cout << "目录已满!" << endl;
            return;
        }

        /* 路径切分解析 */
        string spilt=filepath.substr(0, filepath.find('/'));
        filepath=filepath.substr(filepath.find('/')+1);
        tempDir_index=create_dir(spilt, CREATE);
        if(tempDir_index < 0){
            cout << "路径目录创建失败!" << endl;
            return;
        }
        /* 更新当前文件目录块 */
        write_to_Disc(tempDir_num, temp_block.buffer);
        /* 加载下一级文件目录块 */
        tempDir_num = tempDir->first_block;
        set_File_Pointer(&tempDir, tempDir_num, tempDir_index, temp_block);
    }
}

void File_system::rmdir(string &filepath)
{
    /* 从磁盘中加载当前文件目录块 */
    tempDir_num=currentDir_num;
    tempDir_index=currentDir_index;
    set_File_Pointer(&tempDir, tempDir_num, tempDir_index, temp_block);

    while (true) {
        /* 如果开头为/ */
        if(filepath[0]=='/')
        {
            /* 设置临时目录为根目录 */
            tempDir_num=0;
            tempDir_index=use_num;
            set_File_Pointer(&tempDir, tempDir_num, tempDir_index, temp_block);
            filepath = filepath.substr(1);
        }
        else if(filepath[0]=='.'&&filepath[1]=='/') /* 如果开头为./ */
            filepath = filepath.substr(2);

        /* 如果无下级目录 */
        if(filepath.find('/')==filepath.npos)
        {
            int status = remove_dir(filepath);
            if(status < 0) cout << "目录删除失败!" << endl;
            else cout << "目录删除成功!" << endl;
            return;
        }

        /* 路径切分解析 */
        string spilt=filepath.substr(0, filepath.find('/'));
        filepath=filepath.substr(filepath.find('/')+1);
        tempDir_index=create_dir(spilt, READ_WRITE);
        if(tempDir_index < 0){
            cout << "路径目录不存在!" << endl;
            return;
        }
        /* 更新当前文件目录块 */
        write_to_Disc(tempDir_num, temp_block.buffer);
        /* 加载下一级文件目录块 */
        tempDir_num = tempDir->first_block;
        set_File_Pointer(&tempDir, tempDir_num, tempDir_index, temp_block);
    }

}

void File_system::show_help()
{
    cout << "               My File System Help Message                 " << endl;
    cout << "    ——————————————————————————————————————————————————" << endl;
    cout << "     1.touch <filename>                    -- 新建文件 " << endl;
    cout << "     2.rm <filename>                       -- 删除文件  " << endl;
    cout << "     3.cat <filename>                      -- 查看文件  " << endl;
    cout << "     4.gedit <filename>                    -- 编辑文件  " << endl;
    cout << "     5.mkdir <dirname>                     -- 新建目录  " << endl;
    cout << "     6.rmdir <filename>                    -- 移除目录  " << endl;
    cout << "     7.ls                               -- 显示目录文件  " << endl;
    cout << "     8.cd <dirname>                        -- 进入目录  " << endl;
    cout << "     9.clear                           -- 清除屏幕内容  " << endl;
    cout << "     10.help                               -- 显示帮助  " << endl;
    cout << "     11.exit                               -- 注销用户  " << endl;
    cout << "    ——————————————————————————————————————————————————" << endl;
}


bool all_blank(string &str)
{
    for(int i=0;i<str.length();i++)
        if(str[i]!=' ') return false;
    return true;
}

int main()
{
    system("clear");
    File_system *file_system = new File_system();

    while (true){
        string cmd, op, sname, dname;
        getline(cin, cmd, '\n');
        system("clear");
        cout << "             My File System Help Message     " << endl;
        cout << "    ——————————————————————————————————————————————" << endl;
        cout << "     1.login <name> <pass>         -- 登录         " << endl;
        cout << "     2.register <name> <pass>      -- 注册         " << endl;
        cout << "     3.remove <name> <pass>        -- 移除用户      " << endl;
        cout << "     4.show                        -- 显示用户信息  " << endl;
        cout << "     5.exit                        -- exit        " << endl;
        cout << "    ——————————————————————————————————————————————" << endl;

        getline(cin, cmd, '\n');
        /* 如果只输入空格 */
        if(all_blank(cmd)) continue;
        /* 删除两端空格 */
        cmd = cmd.substr(cmd.find_first_not_of(' '),
                         cmd.find_last_not_of(' ') - cmd.find_first_not_of(' ') + 1);
        if(cmd.length() == 0) continue;

        /* 切分指令 */
        if(cmd.find(" ") == cmd.npos) op = cmd;
        else{
            op = cmd.substr(0, cmd.find(" "));
            dname = cmd.substr(cmd.rfind(" ") + 1);

            /* 判断是否为双输入指令 */
            sname = cmd.substr(cmd.find(" "), cmd.rfind(" ")-cmd.find(" ")+1);
            if(!all_blank(sname)){
                sname = sname.substr(sname.find_first_not_of(' '),
                                     sname.find_last_not_of(' ')-sname.find_first_not_of(' ')+1);
            }
        }

        if(op == "login"){
            if(file_system->log_in(sname, dname)!=USR_EXIST){
                cout << "登录失败！" << endl;
                continue;
            }
        }
        else if(op == "register"){ file_system->Register(sname, dname); continue;}
        else if(op == "remove"){ file_system->Remove(sname, dname); continue;}
        else if(op == "show"){ file_system->show_Usr(); continue;}
        else if(op == "exit"){ break;}
        else continue;
        file_system->show_help();
        file_system->dir_path = "";


        while (true)
        {
            setbuf(stdin, nullptr);
            bool is_cp = false;
            cout << file_system->usr_name << "@ubuntu:" << file_system->dir_path << "$";

            getline(cin, cmd, '\n');

            /* 如果只输入空格 */
            if(all_blank(cmd)) continue;
            /* 删除两端空格 */
            cmd = cmd.substr(cmd.find_first_not_of(' '),
                             cmd.find_last_not_of(' ') - cmd.find_first_not_of(' ') + 1);
            if(cmd.length() == 0) continue;

            /* 切分指令 */
            if(cmd.find(" ") == cmd.npos) op = cmd;
            else{
                op = cmd.substr(0, cmd.find(" "));
                dname = cmd.substr(cmd.rfind(" ") + 1);

                /* 判断是否为cp指令 */
                sname = cmd.substr(cmd.find(" "), cmd.rfind(" ")-cmd.find(" ")+1);
                if(!all_blank(sname)){
                    is_cp = true;
                    sname = sname.substr(sname.find_first_not_of(' '),
                                         sname.find_last_not_of(' ')-sname.find_first_not_of(' ')+1);
                }
            }

            if(op == "touch")  file_system->touch(dname);
            if(op == "rm") file_system->rm(dname);
            if(op == "cat") file_system->cat(dname);
            if(op == "gedit")file_system->gedit(dname);
            if(op == "mkdir") file_system->mkdir(dname);
            if(op == "rmdir") file_system->rmdir(dname);
            if(op == "ls") file_system->list_files();
            if(op == "cd")
                if(file_system->cd_dir(dname)<0)
                    cout << "当前目录不存在!" << endl;
            if(op == "clear") { system("clear"); file_system->show_help(); }
            if(op == "help") file_system->show_help();
            if(op == "exit") { file_system->log_out();break; }
            file_system->update_zero_block();
        }
    }
    delete file_system;
    return 0;
}

