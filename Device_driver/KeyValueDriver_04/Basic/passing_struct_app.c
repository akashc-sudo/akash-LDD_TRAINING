#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#define PATH "/dev/simple_device"

struct data
{
	int var1;
	int var2;
};

int main()
{
	struct data user;
	struct data *from_kernel;
	from_kernel = malloc(sizeof(struct data));
	user.var1 = 100;
	user.var2 = 200;

	int fd =open(PATH ,O_WRONLY);
	perror("open");
	write(fd,&user,sizeof(user));
	perror("write");
	close(fd);

	close(fd);
        fd = open(PATH ,O_RDONLY);
	read(fd,from_kernel,sizeof(from_kernel));
	perror("read");
        //printf("%s\n",from_kernel);
	printf("var1 = %d\nvar2 = %d\n",from_kernel->var1,from_kernel->var2);
}

