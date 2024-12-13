#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>

#define MSG 5
#define PATH "/dev/SimpleDevice"

int main()
{
      char user_str[50],read_buff[50];
      int fd = open(PATH,O_WRONLY);
          if(fd < 0)
	  {
		printf("Not able to open the file\n");
		return 0;
	  }
	  
	  printf("Enter the 5 msg to buffer each msg should be ended with space\n");
	  for(int i = 0 ; i < MSG ;i++)
	  {
	  scanf("%s",user_str);
	  write(fd,user_str,strlen(user_str));
	  }
	  perror("write");
	  close(fd);
          int op;
          fd = open(PATH,O_RDONLY);
	  if(fd < 0){ printf("not able to open rthe file\n"); 
		  return 0;
	  }
	  while(1)
          {
          printf("Enter the options given below\n1.To get oldest msg from the buffer\n2.To get all the msgs from  buffer\n3.To Add new msg to buffer\n 4.To EXit the program\n");
	  scanf("%d",&op);
	  switch(op)
	  {
		           
		  case 1 :fd = open(PATH,O_RDONLY);
	                  if(fd < 0){ printf("not able to open rthe file\n"); 
		                      return 0;}
		           read(fd,read_buff,sizeof(read_buff));
			   perror("read");
			   printf("Oldest msg ofbuffer = %s\n",read_buff);
			   break;
                  case 2 : for(int i = 0 ; i < MSG ;i++)
	                   {     
				 bzero(read_buff,sizeof(read_buff));
	 			 read(fd,read_buff,sizeof(read_buff));
	  			 perror("read");
	 			 printf("%s\n",read_buff);
			   }
			   break;
		  case 3 : close(fd);
			   printf("Enter a new  msg to buffer\n");
			   scanf("%s",user_str);
			   fd = open(PATH,O_WRONLY);
		           write(fd,user_str,strlen(user_str));
                           perror("write");
			   break;
			   
		 case 4 : exit(0);   
		           	   
          } 
	  }
}
