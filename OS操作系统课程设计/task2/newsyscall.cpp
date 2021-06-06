
asmlinkage long sys_copyfile(const char __user* source, const char __user* target);

asmlinkage long sys_copyfile(const char __user* source, const char __user* target)
{
    int from_fd, to_fd;
    int bytes_read,bytes_write;
    char buffer[1024];
    char *ptr;

    /* 解除内核对用户地址的访问检查 */
    mm_segment_t old_fs = get_fs();
    set_fs(KERNEL_DS);

    /* 打开源文件与目标文件 */
    from_fd = sys_open(source, O_RDONLY, S_IRUSR);
    if (from_fd <= 0){
        printk("source path error!\n");
        set_fs(old_fs);
        return -1;
    }
    to_fd = sys_open(target, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
    if (to_fd <= 0){
        printk("target path error!\n");
        set_fs(old_fs);
        return -1;
    }

    /* 拷贝文件 */
    while((bytes_read=sys_read(from_fd, buffer, 1024)))
    {
        /* 一个致命的错误发生了 */
        if(bytes_read==-1) break;
        else if(bytes_read>0)
        {
            ptr=buffer;
            while((bytes_write=sys_write(to_fd, ptr, bytes_read)))
            {
                /* 一个致命错误发生了 */
                if(bytes_write==-1)break;
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

    sys_close(from_fd);
    sys_close(to_fd);

    /* 读写错误 */
    if(bytes_read==-1||bytes_write==-1){
        printk("read/write error!\n");
        return -1;
    }
    set_fs(old_fs);
    return 1;
}



