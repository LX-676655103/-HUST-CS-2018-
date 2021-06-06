
asmlinkage long sys_copyfile(const char __user* source, const char __user* target);

asmlinkage long sys_copyfile(const char __user* source, const char __user* target)
{
    int from_fd, to_fd;
    int bytes_read,bytes_write;
    char buffer[1024];
    char *ptr;

    /* ����ں˶��û���ַ�ķ��ʼ�� */
    mm_segment_t old_fs = get_fs();
    set_fs(KERNEL_DS);

    /* ��Դ�ļ���Ŀ���ļ� */
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

    /* �����ļ� */
    while((bytes_read=sys_read(from_fd, buffer, 1024)))
    {
        /* һ�������Ĵ������� */
        if(bytes_read==-1) break;
        else if(bytes_read>0)
        {
            ptr=buffer;
            while((bytes_write=sys_write(to_fd, ptr, bytes_read)))
            {
                /* һ�������������� */
                if(bytes_write==-1)break;
                /* д�������ж����ֽ� */
                else if(bytes_write==bytes_read) break;
                /* ֻд��һ����,����д */
                else if(bytes_write>0)
                {
                    ptr+=bytes_write;
                    bytes_read-=bytes_write;
                }
            }
            /* д��ʱ�������������� */
            if(bytes_write==-1)break;
        }
    }

    sys_close(from_fd);
    sys_close(to_fd);

    /* ��д���� */
    if(bytes_read==-1||bytes_write==-1){
        printk("read/write error!\n");
        return -1;
    }
    set_fs(old_fs);
    return 1;
}



