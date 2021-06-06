#include <linux/kernel.h>
#include <linux/module.h>

#include <linux/fs.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/uaccess.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/slab.h>

#define MAX_SIZE 1024

int my_open(struct inode *inode, struct file *file);
int my_release(struct inode *inode, struct file *file);
ssize_t my_read(struct file *fileP, char *buf, size_t count, loff_t *ppos);
ssize_t my_write(struct file *fileP, const char *buf, size_t count, loff_t *ppos);

char *data = NULL;
int device_num;//设备号
char* devName = "my_drive";//设备名



/* 注册 */
int _my_init_module(void)
{
    device_num = register_chrdev(0, devName, &pStruct);
    if (device_num < 0)
    {
        printk("failed to register my drive.\n");
        return -1;
    }
    printk("my drive has been registered!\n");
    printk("id: %d\n", device_num);
    return 0;
}

/* 注销 */
void _my_cleanup_module(void)
{
    unregister_chrdev(device_num, devName);
    printk("unregister successful.\n");
}


/* 打开 */
int my_open(struct inode *inode, struct file *file)
{
    try_module_get(THIS_MODULE);
    printk("module_refcount(module):%d\n",module_refcount(THIS_MODULE));

    data = (char*)kmalloc(sizeof(char) * MAX_SIZE, GFP_KERNEL);
    if (!data)  return -ENOMEM;
    memset(data, 0, MAX_SIZE);
    printk("my_drive open successful!\n");
    return 0;
}

/* 关闭 */
int my_release(struct inode *inode, struct file *file)
{
    module_put(THIS_MODULE);
    printk("module_refcount(module):%d\n",module_refcount(THIS_MODULE));
    printk("Device released!\n");
    if (data)
    {
        kfree(data);
        data = NULL;
    }
    return 0;
}


/* 读数据 */
ssize_t my_read(struct file *fileP, char *buf, size_t count, loff_t *ppos)
{
    if (!buf) return -EINVAL;
    if (count > MAX_SIZE) count = MAX_SIZE;
    if (count < 0 ) return -EINVAL;

	if (copy_to_user(buf, data, count) == EFAULT)
		return -EFAULT;

	printk("user read data from device!\n");
	return count;
}

/* 写数据 */
ssize_t my_write(struct file *fileP, const char *buf, size_t count, loff_t *ppos)
{
    if (!buf) return -EINVAL;
    if (count > MAX_SIZE) count = MAX_SIZE;
    if (count < 0 ) return -EINVAL;

    memset(data, 0, MAX_SIZE);
	if (copy_from_user(data, buf, count) == EFAULT)
		return -EFAULT;

	printk("user write data to device\n");
	return count;
}

struct file_operations pStruct =
{
    owner:THIS_MODULE,
    open:my_open,
    release:my_release,
    read:my_read,
    write:my_write,
};
module_init(_my_init_module);
module_exit(_my_cleanup_module);

MODULE_AUTHOR("lql");
MODULE_LICENSE("GPL");


