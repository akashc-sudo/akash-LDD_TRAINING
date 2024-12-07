/*	
 *	user application program to do operation on gpio17 Pin of rpi  and retrive the current status of gpio17
 *	before using check associated driver is loaded.
 **/


#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>

#define PATH "/dev/GpioDevice"


int main()
{
	char op;
	printf("Enter the op you want to do on GPIO_17\n 0 TO SET LOW\n 1 TO SET HIGH\n 2 TO RETRIVE CURRENT STATUS\n");
	scanf("%c",&op);

	switch(op)
	{
		case '0' : int fd = open(PATH,O_WRONLY);
			     if(fd < 0){printf("NOT ABLE TO OPEN FILE\n");return 0;}
			     write(fd,&op,sizeof(op));
			     perror("write");
			     close(fd);
			     break;
                             
			     /*		This case getting small bug	*/

			     
		case '1' :   //char str[2]="1";
			     open(PATH,O_WRONLY);
			     if(fd < 0){printf("NOT ABLE TO OPEN FILE\n");return 0;}
			     write(fd,&op,sizeof(op));
			     perror("write");
			     close(fd);
			     break;

	         	      
		case '2' :   fd = open(PATH,O_RDONLY);
			     if(fd < 0){printf("NOT ABLE TO OPEN FILE\n");return 0;}
			     read(fd,&op,sizeof(op));
			     perror("read");
			     printf("Retrived current status of GPIO17 = %c\n",op);
			     close(fd);
			     break;

	
                 default :  printf("InValid Option try again..........! ");
                            break;
	}
}	


