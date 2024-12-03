#include<stdio.h>
#include<string.h>
#include<fcntl.h>
#include<stdlib.h>
#include<unistd.h>
#define PATH "/dev/SimpleDevice"
int main()
{
	int num;
	char str[3][10],rec[100];
	int fd = open(PATH,O_WRONLY);
            printf("Enter the opertion and values you can follow pattern as mentioned below\n");
	    printf("operation<sub sum mul> num1<any integer value> num2<integer value>\n");
	    for(int i = 0 ; i < 3 ;i++)
	    {
		    scanf("%s",str[i]);
	    }

	    printf("Requesting.............!\n");
	    write(fd,str,strlen(str));
	    perror("write");
	    close(fd);	

	    fd = open(PATH,O_RDONLY);
	    printf("Ready to Read..........!\n");
	    read(fd,rec,sizeof(rec));
	    perror("read");
	    printf("Result of %s  =  %d\n",str[0],atoi(rec));
	    printf("DONE...................!\n");
}


