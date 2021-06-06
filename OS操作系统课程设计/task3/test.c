#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/errno.h>

#define DEV_NAME "/dev/my_drive"
#define BUF_SIZE 1024


int main(int argc, char const *argv[])
{
	int fd;
	char buffer[BUF_SIZE];

	fd = open(DEV_NAME, O_RDWR);
	if (fd < 0) {
		printf("open dev fail!\n");
		return -1;
	}

    do {
    	printf("Input some worlds to kernel(enter 'quit' to exit)\n");
    	memset(buffer, 0 ,BUF_SIZE);
    	if (fgets(buffer, BUF_SIZE, stdin) == NULL) {
    		printf("fgets error!\n");
    		break;
    	}
    	buffer[strlen(buffer) - 1] = '\0';
    	if (write(fd, buffer, strlen(buffer)) < 0) {
    		printf("write error\n");
    		break;
    	}
    	if (read(fd, buffer, BUF_SIZE) < 0) {
    		printf("read error\n");
    		break;
    	}
    	else printf("The read string is from kernel : %s\n", buffer);
    } while(strncmp(buffer, "quit" , 4));

    close(fd);
	return 0;
}
