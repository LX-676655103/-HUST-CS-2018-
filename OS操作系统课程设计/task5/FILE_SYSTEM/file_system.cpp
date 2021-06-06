#include "file_system.h"
File* tempDir;     /* 临时目录-用于路径解析 */
bool op_flag;

File_system::~File_system()
{
    fclose(fdisc);
}

File_system::File_system()
{
    /* 创建磁盘文件
     * 0号块为引导块 */
    fdisc = NULL;
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
void File_system::read_from_Disc(int block, char* buf)
{
    fseek(fdisc, block*BLOCK_SIZE, SEEK_SET);
    fread(buf, sizeof(char), BLOCK_SIZE, fdisc);
}

void File_system::write_to_Disc(int block, char* buf)
{
    fseek(fdisc, block*BLOCK_SIZE, SEEK_SET);
    fwrite(buf, sizeof(char), BLOCK_SIZE, fdisc);
}

int File_system::get_blank_block1()
{
    char buf[BLOCK_SIZE],temp_buf[BLOCK_SIZE];
    read_from_Disc(0, buf);
    int temp_bk=*((int*)buf);
    int bk_nfree = ((int*)buf)[1];
    if(bk_nfree==0)
    {
        cout << "存储空间耗尽！";
        return 0;
    }
    else
    {
        read_from_Disc(temp_bk, temp_buf);
        *((int*)buf) = *((int*)temp_buf);
        ((int*)buf)[1]-=1;
        write_to_Disc(0, buf);
        return temp_bk;
    }
}

int File_system::Register(string &user, string &pass)
{
    int usr_num = 0;
    char buf[BLOCK_SIZE];
    /* 读取引导块信息 */
    read_from_Disc(0, buf);
    Usr* currUsr = (Usr*)&buf[8];
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
    if(usr_num==MAX_USR_NUM)
        return USR_FULL;
    /* 创建用户 */
    int root = get_blank_block1();
    if(!root) return USR_CREATE_FAIL;
    read_from_Disc(0, buf);
    currUsr = (Usr*)&buf[8];

    currUsr[usr_num].root_block = root;
    currUsr[usr_num].vaild=1;
    strcpy(currUsr[usr_num].usrname,user.c_str());
    strcpy(currUsr[usr_num].passname,pass.c_str());


    /* 添加 .与..目录 */
    currUsr[usr_num].root_size = 64;
    char filebuf[BLOCK_SIZE];
    read_from_Disc(currUsr[usr_num].root_block, filebuf);

    File* pdir = (File*)filebuf; /* 使用文件指针pdir进行文件操作 */
    strcpy(pdir[0].name,".");
    pdir[0].file_size = usr_num;
    pdir[0].file_type = DIR_TYPE;
    pdir[0].first_block = 0;

    strcpy(pdir[1].name,"..");
    pdir[1].file_size = 0;
    pdir[1].file_type = DIR_TYPE;
    pdir[1].first_block = 0;

    write_to_Disc(currUsr[usr_num].root_block, filebuf);

    /* 将修改结果写回 */
    write_to_Disc(0, buf);
    return USR_CREATE_SUCCESS;
}

int File_system::log_in(string &user, string &pass)
{

    int usr_num = 0;
    char buf[BLOCK_SIZE];
    /* 读取引导块信息 */
    read_from_Disc(0, buf);
    Usr* currUsr = (Usr*)&buf[8];
    /* 查找当前是否存在该用户 */
    for(;usr_num<MAX_USR_NUM;usr_num++){
        if(!currUsr[usr_num].vaild) return USR_NOT_EXIST;
        if(!strcmp(currUsr[usr_num].usrname, user.c_str()))
        {
            cout << "用户名正确！" << endl;
            if(!strcmp(currUsr[usr_num].passname, pass.c_str()))
            {
                cout << "密码正确！" << endl;
                use_num = usr_num;
                /* 设置当前的根目录 */
                rootDir = new File();
                rootDir->file_type = DIR_TYPE;
                rootDir->first_block = currUsr[usr_num].root_block;
                rootDir->file_size = currUsr[usr_num].root_size;
                tempDir = rootDir;
                strcpy(rootDir->name,"root");
                currentDir=rootDir;
                tempDir=currentDir;
                /* 加载磁盘 */
                fseek(fdisc, 0, SEEK_SET);
                for(int i=0;i<STORAGE_SIZE;i++){
                    fread(storage.block[i].buffer, sizeof(char), BLOCK_SIZE, fdisc);
                }
                read_from_Disc(0, buf);
                bk_fhead = *((int*)buf);  /* 空白块头指针 */
                bk_nfree = ((int*)buf)[1];  /* 空白块数 */

                return USR_EXIST;
            }

        }
    }
    return USR_NOT_EXIST;
}

int File_system::log_out()
{
    char buf[BLOCK_SIZE];
    /* 更新根目录的尺寸 */
    /* 更新磁盘 */
    fseek(fdisc, BLOCK_SIZE, SEEK_SET);
    for(int i=1;i<STORAGE_SIZE;i++){
        fwrite(storage.block[i].buffer, sizeof(char), BLOCK_SIZE, fdisc);
    }
    fseek(fdisc, 0, SEEK_SET);
    read_from_Disc(0, buf);
    *((int*)buf) = bk_fhead;   /* 空白块头指针 */
    ((int*)buf)[1] = bk_nfree;  /* 空白块数 */
    Usr* currUsr = (Usr*)&buf[8];
    currUsr[use_num].root_size = rootDir->file_size;

    //cout << "tips: " << currUsr[use_num].root_size << endl;
    write_to_Disc(0, buf);
    return 0;
}

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
        cout << "root: "<< currUsr[usr_num].root_block << endl << endl;
    }

}

int File_system::Remove(string &user, string &pass)
{
    int num = log_in(user, pass);
    if(num == USR_EXIST) ;
    else cout << "当前用户不存在！" << endl;
    delete_dir_dfs(rootDir->first_block, rootDir->file_size/32-1);
    log_out();

    char buf[BLOCK_SIZE];
    /* 读取引导块信息 */
    read_from_Disc(0, buf);
    Usr* currUsr = (Usr*)&buf[8];
    int i=use_num;

    cout << "use_num: " << i << endl;

    for(;i<MAX_USR_NUM;i++)
    {
        if(!currUsr[i+1].vaild) break;
        currUsr[i] = currUsr[i+1];
    }
    currUsr[i].vaild = 0;
    write_to_Disc(0, buf);
}

void File_system::touch(string &filepath)
{
    int filenum;
    string spilt;
    tempDir=currentDir;
    while (true) {
        /* 如果开头为.或./ */
        if(filepath[0]=='/') filepath = filepath.substr(1);
        else if(filepath[0]=='.'&&filepath[1]=='/')
            filepath = filepath.substr(2);

        /* 如果无下级目录 */
        if(filepath.find('/')==filepath.npos)
        {
            op_flag = TOUCH_OP;
            int status = create_file(filepath, filenum);
            if(status==FILE_EXIST) cout << "当前文件已存在！" << endl;
            else if(status==FILE_CREATE_SUCCESS) cout << "文件创建成功！" << endl;
            else cout << "文件创建失败！" << endl;
            break;
        }

        /* 路径切分解析 */
        spilt=filepath.substr(0, filepath.find('/'));
        filepath=filepath.substr(filepath.find('/'));
        op_flag = MKDIR_OP;
        if(create_dir(spilt, filenum)==DIR_CREATE_FAILED){
            cout << "目录创建失败！" << endl;
            break;
        }
        tempDir=&((File*)storage.block[tempDir->first_block].buffer)[filenum];
    }
}

int File_system::catch_file(string &filepath)
{
    int filenum;
    string spilt;
    tempDir=currentDir;
    while (true) {
        /* 如果开头为.或./ */
        if(filepath[0]=='/') filepath = filepath.substr(1);
        else if(filepath[0]=='.'&&filepath[1]=='/')
            filepath = filepath.substr(2);

        /* 如果无下级目录 */
        if(filepath.find('/')==filepath.npos)
        {
            int find_num=open_file(filepath);
            if(find_num==-1) return -1;
            if(((File*)storage.block[tempDir->first_block].buffer)[find_num].file_type==DIR_TYPE){
                cout << "目录文件无法显示！" << endl;
                return -1;
            }

            /* 输出原有文本 */
            int tempblock=((File*)storage.block[tempDir->first_block].buffer)[find_num].first_block;
            int tempsize=((File*)storage.block[tempDir->first_block].buffer)[find_num].file_size;
            cout << "当前文本内容如下:" << endl;
            int block_length=BLOCK_SIZE-4;
            while(tempsize!=0)
            {
                if(!block_length){
                    block_length=BLOCK_SIZE-4;
                    tempblock=*((int*)storage.block[tempblock].buffer);
                }
                cout << storage.block[tempblock].buffer[BLOCK_SIZE-block_length];
                tempsize--;
                block_length--;
            }
            return 0;
        }

        /* 路径切分解析 */
        spilt=filepath.substr(0, filepath.find('/'));
        filepath=filepath.substr(filepath.find('/'));
        op_flag = CD_OP;
        if(create_dir(spilt, filenum)==DIR_NOT_EXIST){
            cout << "路径不存在！" << endl;
            break;
        }
        tempDir=&((File*)storage.block[tempDir->first_block].buffer)[filenum];
    }
}

int File_system::gedit_file(string &filepath)
{
    int filenum;
    string spilt;
    tempDir=currentDir;
    while (true) {
        /* 如果开头为.或./ */
        if(filepath[0]=='/') filepath = filepath.substr(1);
        else if(filepath[0]=='.'&&filepath[1]=='/')
            filepath = filepath.substr(2);

        /* 如果无下级目录 */
        if(filepath.find('/')==filepath.npos)
        {
            int find_num=open_file(filepath);
            if(find_num==-1) return -1;
            if(((File*)storage.block[tempDir->first_block].buffer)[find_num].file_type==DIR_TYPE){
                cout << "目录文件无法编辑！" << endl;
                return -1;
            }

            /* 输出原有文本 */
            int tempblock=((File*)storage.block[tempDir->first_block].buffer)[find_num].first_block;
            int tempsize=((File*)storage.block[tempDir->first_block].buffer)[find_num].file_size;
            cout << "当前文本内容如下:" << endl;
            int block_length=BLOCK_SIZE-4;
            while(tempsize!=0)
            {
                if(!block_length){
                    block_length=BLOCK_SIZE-4;
                    tempblock=*((int*)storage.block[tempblock].buffer);
                }
                cout << storage.block[tempblock].buffer[BLOCK_SIZE-block_length];
                tempsize--;
                block_length--;
            }

            /* 读入新的文本 */
            cout << "请输入需要输入的文本(Ctrl+Z结尾):" << endl;
            char c;
            block_length=BLOCK_SIZE-4;

            int temp_block=get_blank_block();
            int head_block=temp_block;
            *((int*)storage.block[temp_block].buffer)=0;
            if(!temp_block) return -1;
            tempsize=0;

            while((c=getchar())!=EOF)
            {
                if(!block_length){
                    block_length=BLOCK_SIZE-4;
                    int temp=get_blank_block();
                    if(!temp) return -1;
                    *((int*)storage.block[temp_block].buffer)=temp;
                    temp_block=temp;
                }
                storage.block[temp_block].buffer[BLOCK_SIZE-block_length]=c;
                block_length--;
                tempsize++;
            }
            *((int*)storage.block[temp_block].buffer)=0;


            /*释放原有的空白块*/
            ((File*)storage.block[tempDir->first_block].buffer)[find_num].file_size=tempsize;
            int block_num=((File*)storage.block[tempDir->first_block].buffer)[find_num].first_block;
            while(block_num!=0)
            {
                int tempnum=*((int*)storage.block[block_num].buffer);
                *((int*)storage.block[block_num].buffer)=bk_fhead;
                bk_fhead=block_num;
                block_num=tempnum;
                bk_nfree++;
            }
            ((File*)storage.block[tempDir->first_block].buffer)[find_num].first_block=head_block;
            return 0;
        }

        /* 路径切分解析 */
        spilt=filepath.substr(0, filepath.find('/'));
        filepath=filepath.substr(filepath.find('/'));
        op_flag = CD_OP;
        if(create_dir(spilt, filenum)==DIR_NOT_EXIST){
            cout << "路径不存在！" << endl;
            break;
        }
        tempDir=&((File*)storage.block[tempDir->first_block].buffer)[filenum];
    }

}
int File_system::create_file(string &filename, int &filenum)
{
    filenum = -1;
    /* filelist指向文件目录项 */
    File* filelist = (File*)storage.block[tempDir->first_block].buffer;
    int file_num = tempDir->file_size/32;

    /* 查询是否已经存在该文件 */
    for(int i=0;i<file_num;i++){
        if(!strcmp(filelist[i].name,filename.c_str()))
        {
            filenum = i;
            if(filelist[i].file_type==FILE_TYPE)
                return FILE_EXIST;
            else{
                cout << "该文件名与目录名冲突！" << endl;
                return FILE_CREATE_FAILED;
            }
        }
    }
    if(op_flag == CAT_OP&&filenum==-1) return FILE_NOT_EXIST;

    if(tempDir->file_size>=1024) return FILE_CREATE_FAILED;
    else{
        if(bk_nfree==0) cout << "存储空间耗尽！";
        else{
            File* newFile = &filelist[file_num];
            tempDir->file_size += 32;

            /*  从磁盘申请空白块 */
            int newblock = get_blank_block();
            if(!newblock)
                return FILE_CREATE_FAILED;
            strcpy(newFile->name,filename.c_str());
            newFile->first_block=newblock;
            *((int*)storage.block[newFile->first_block].buffer)=0;
            newFile->file_size = 0;
            newFile->file_type = FILE_TYPE;
            filenum = file_num;
            return FILE_CREATE_SUCCESS;

        }
    }

}
int File_system::delete_file(string &filepath)
{
    int filenum;
    string spilt;
    tempDir=currentDir;
    while (true) {
        /* 如果开头为.或./ */
        if(filepath[0]=='/') filepath = filepath.substr(1);
        else if(filepath[0]=='.'&&filepath[1]=='/')
            filepath = filepath.substr(2);

        /* 如果无下级目录 */
        if(filepath.find('/')==filepath.npos)
        {
            /* 查找需要删除的文件 */
            int find_flag=false;
            int find_num=-1;
            for(int i=0;i<tempDir->file_size/32;i++)
                if(strcmp(((File*)storage.block[tempDir->first_block].buffer)[i].name,filepath.c_str())==0)
                    find_flag=true,find_num=i;
            if(!find_flag){
                cout << "当前文件不存在！" << endl;
                return -1;
            }

            File* tempFile=&((File*)storage.block[tempDir->first_block].buffer)[find_num];
            if(tempFile->file_type==DIR_TYPE){
                cout << "当前文件为目录！" << endl;
                return 0;
            }
            int block_num=tempFile->first_block;
            while(block_num!=0)
            {
                int tempnum=*((int*)storage.block[block_num].buffer);
                *((int*)storage.block[block_num].buffer)=bk_fhead;
                bk_fhead=block_num;
                block_num=tempnum;
                bk_nfree++;
            }
            while (find_num<tempDir->file_size/32-1) {
                ((File*)storage.block[tempDir->first_block].buffer)[find_num]=
                        ((File*)storage.block[tempDir->first_block].buffer)[find_num+1];
                find_num++;
            }
            tempDir->file_size-=32;
            return 0;
        }

        /* 路径切分解析 */
        spilt=filepath.substr(0, filepath.find('/'));
        filepath=filepath.substr(filepath.find('/'));
        op_flag = CD_OP;
        if(create_dir(spilt, filenum)==DIR_NOT_EXIST){
            cout << "路径不存在！" << endl;
            break;
        }
        tempDir=&((File*)storage.block[tempDir->first_block].buffer)[filenum];
    }
}

int File_system::open_file(string &filename)
{
    for(int i=0;i<tempDir->file_size/32;i++)
        if(strcmp(((File*)storage.block[tempDir->first_block].buffer)[i].name,
                  filename.c_str())==0)
            return i;

    cout << "当前文件不存在！" << endl;
    return -1;
}
int File_system::copy_file(string &frompath, string &topath)
{
    return 0;
}

/* 目录操作 */
void File_system::mkdir(string &filepath)
{
    int filenum;
    string spilt, temp_name;
    tempDir=currentDir;
    while (true) {
        /* 如果开头为.或./ */
        if(filepath[0]=='/') filepath = filepath.substr(1);
        else if(filepath[0]=='.'&&filepath[1]=='/')
            filepath = filepath.substr(2);

        /* 如果无下级目录 */
        if(filepath.find('/')==filepath.npos)
        {
            op_flag = MKDIR_OP;
            int status = create_dir(filepath, filenum);
            if(status==DIR_EXIST) cout << "当前目录已存在！" << endl;
            else if(status==DIR_CREATE_SUCCESS) cout << "目录创建成功！" << endl;
            else cout << "目录创建失败！" << endl;
            break;
        }

        /* 路径切分解析 */
        spilt=filepath.substr(0, filepath.find('/'));
        filepath=filepath.substr(filepath.find('/'));
        op_flag = MKDIR_OP;
        if(create_dir(spilt, filenum)==DIR_CREATE_FAILED){
            cout << "目录创建失败！" << endl;
            break;
        }
        tempDir=&((File*)storage.block[tempDir->first_block].buffer)[filenum];
    }
}
int File_system::create_dir(string &filename, int &filenum) //创建目录
{
    filenum = -1;
    /* filelist指向文件目录项 */
    File* filelist = (File*)storage.block[tempDir->first_block].buffer;
    int file_num = tempDir->file_size/32;

    /* 查询是否已经存在该目录 */
    for(int i=2;i<file_num;i++){
        if(!strcmp(filelist[i].name,filename.c_str()))
        {
            filenum = i;
            if(filelist[i].file_type==DIR_TYPE)
                return DIR_EXIST;
            else{
                cout << "该目录名与文件名冲突！" << endl;
                return DIR_CREATE_FAILED;
            }
        }
    }
    if(op_flag == CD_OP&&filenum==-1)
        return DIR_NOT_EXIST;



    /* 当前目录不存在，需要创建当前目录
     * 若当前目录已满，则创建失败
     * 否则，将创建当前目录 */
    if(tempDir->file_size>=1024) return DIR_CREATE_FAILED;
    else{
        File* newDir = &filelist[file_num];
        tempDir->file_size += 32;
        /*  从磁盘申请空白块 */
        int newblock = get_blank_block();
        if(!newblock)
            return DIR_CREATE_FAILED;
        strcpy(newDir->name,filename.c_str());

        newDir->file_type = DIR_TYPE;
        newDir->first_block = newblock;
        /* 添加 .与..目录 */
        newDir->file_size = 64;
        File* pdir = (File*)storage.block[newblock].buffer; /* 使用文件指针pdir进行文件操作 */

        strcpy(pdir[0].name,".");
        pdir[0].file_size = file_num;
        pdir[0].file_type = DIR_TYPE;
        pdir[0].first_block = tempDir->first_block;

        strcpy(pdir[1].name,"..");
        pdir[1].file_size = filelist[0].file_size;
        pdir[1].file_type = DIR_TYPE;
        pdir[1].first_block = filelist[0].first_block;

        filenum = file_num;
        return DIR_CREATE_SUCCESS;
    }
}
void File_system::rmdir(string &filepath)
{
    int filenum;
    string spilt;
    tempDir=currentDir;
    while (true) {
        /* 如果开头为.或./ */
        if(filepath[0]=='/') filepath = filepath.substr(1);
        else if(filepath[0]=='.'&&filepath[1]=='/')
            filepath = filepath.substr(2);

        /* 如果无下级目录 */
        if(filepath.find('/')==filepath.npos)
        {
            int status = delete_dir(filepath, filenum);
            if(status==DIR_EXIST)
            {
                File* temp = tempDir;
                File* tempFile=&((File*)storage.block[tempDir->first_block].buffer)[filenum];
                int block_num=tempFile->first_block;
                delete_dir_dfs(block_num, tempFile->file_size/32-1);

                tempDir = temp;
                while (filenum<tempDir->file_size/32-1) {
                    ((File*)storage.block[tempDir->first_block].buffer)[filenum]=
                            ((File*)storage.block[tempDir->first_block].buffer)[filenum+1];
                    filenum++;
                }
                tempDir->file_size-=32;

            }
            else cout << "当前目录不存在！" << endl;
            break;
        }

        /* 路径切分解析 */
        spilt=filepath.substr(0, filepath.find('/'));
        filepath=filepath.substr(filepath.find('/'));
        if(delete_dir(spilt, filenum)==DIR_NOT_EXIST)
        {
            cout << "目录路径不存在！" << endl;
            break;
        }
        tempDir=&((File*)storage.block[tempDir->first_block].buffer)[filenum];
    }

}
int File_system::delete_dir_dfs(int block, int file_num)
{
    /* filelist指向文件目录项 */
    File* filelist = (File*)storage.block[block].buffer;
    tempDir=(File*)storage.block[block].buffer;
    for(int i=2;i<=file_num;i++)
    {
        string name = filelist[i].name;
        if(filelist[i].file_type==FILE_TYPE)
            delete_file(name);
        if(filelist[i].file_type==DIR_TYPE){
            delete_dir_dfs(filelist[i].first_block, filelist[i].file_size/32-1);
            cout << "目录" << name << "删除！" << endl;
        }
    }

    *((int*)storage.block[block].buffer)=bk_fhead;
    bk_fhead=block;
    bk_nfree++;
    return 0;
}

int File_system::delete_dir(string &filename, int &filenum) //删除目录
{
    filenum = -1;
    /* filelist指向文件目录项 */
    File* filelist = (File*)storage.block[tempDir->first_block].buffer;
    int file_num = tempDir->file_size/32;
    /* 查询是否已经存在该目录 */
    for(int i=0;i<file_num;i++){
        if(!strcmp(filelist[i].name,filename.c_str()))
        {
            filenum = i;
            if(filelist[i].file_type==DIR_TYPE) return DIR_EXIST;
            else return DIR_NOT_EXIST;
        }
    }
    return DIR_NOT_EXIST;
}


int File_system::catch_dir() //显示目录内容
{
    cout << currentDir->name << ":" << endl;
    Block *tempblock=&storage.block[currentDir->first_block];
    for(int i=2;i<currentDir->file_size/32;i++){
        if(((File*)tempblock->buffer)[i].file_type==FILE_TYPE) cout << "FILE ";
        else cout << "DIR  ";
        cout << ((File*)tempblock->buffer)[i].name << " ";
        cout << ((File*)tempblock->buffer)[i].file_size;
        cout <<"B  ";
        cout <<"blocknum: " << ((File*)tempblock->buffer)[i].first_block << endl;
    }
    return 0;
}
void File_system::cd_dir(string &filepath)
{
    int filenum;
    string spilt, temp_name;
    tempDir=currentDir;
    while (true) {
        /* 如果开头为.或./ */
        if(filepath[0]=='/') filepath = filepath.substr(1);
        else if(filepath[0]=='.'&&filepath[1]=='/')
            filepath = filepath.substr(2);

        /* 如果无下级目录 */
        if(filepath.find('/')==filepath.npos)
        {
            op_flag = CD_OP;
            int status = create_dir(filepath, filenum);
            if(status==DIR_EXIST){
                currentDir=&((File*)storage.block[tempDir->first_block].buffer)[filenum];
                /* 刷新..目录以及.目录 */
                string tempstr = currentDir->name;
                if(tempstr==".."){
                    if(!currentDir->first_block) currentDir = rootDir;
                    else currentDir = &((File*)storage.block[currentDir->first_block].buffer)
                            [currentDir->file_size];

                    dir_path=dir_path.substr(0,dir_path.length()-1);
                    dir_path=dir_path.substr(0,dir_path.find_last_of("/")+1);
                }
                else dir_path += (tempstr + "/");
            }
            else if(status==DIR_NOT_EXIST)
                cout << "目录不存在！" << endl;
            break;
        }

        /* 路径切分解析 */
        spilt=filepath.substr(0, filepath.find('/'));
        filepath=filepath.substr(filepath.find('/'));
        op_flag = CD_OP;
        if(create_dir(spilt, filenum)==DIR_NOT_EXIST){
            cout << "目录路径不存在！" << endl;
            break;
        }
        tempDir=&((File*)storage.block[tempDir->first_block].buffer)[filenum];
        string tempstr = tempDir->name;
        if(tempstr==".."){
            currentDir = (File*)currentDir->file_size;
            dir_path=dir_path.substr(0,dir_path.length()-1);
            dir_path=dir_path.substr(0,dir_path.find_last_of("/")+1);
        }
        else dir_path += (tempstr + "/");
    }
}
/* 其他操作 */
int File_system::get_blank_block()
{
    int temp_bk=bk_fhead;
    if(bk_nfree==0) {
        cout << "存储空间耗尽！";
        return 0;
    }
    else{
        bk_fhead = *((int*)storage.block[bk_fhead].buffer);
        bk_nfree--;
        return temp_bk;
    }
}
void File_system::show_help()
{
    cout << "                   My File System Help Message                 " << endl;
    cout << "    ———————————————————————————————" << endl;
    cout << "     1.touch <filename> -- 新建文件 2.rm <filename> -- 删除文件  " << endl;
    cout << "     3.cp <filename> -- (未实现)    4.cat <filename> -- 查看文件 " << endl;
    cout << "     5.gedit <filename> -- 编辑文件 6.mkdir <dirname> -- 新建目录" << endl;
    cout << "     7.rmdir <filename> -- 移除目录 8.ls -- 显示目录文件         " << endl;
    cout << "     9.cd <dirname> -- 进入目录     10.clear -- 清除屏幕内容     " << endl;
    cout << "     11.help -- 显示帮助            12.exit -- 注销用户              " << endl;
    cout << "    ———————————————————————————————" << endl;
}





















