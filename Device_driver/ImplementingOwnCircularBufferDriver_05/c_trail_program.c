#include<stdio.h>
#include<string.h>
#include<stdlib.h>
int main()
{
	char str[4][10];
        //str = malloc(sizeof(char)*10);
	char storing_buffer[2][10];
	printf("Enter the msg\n");
	for(int i =0 ; i < 4 ;i++)
	scanf("%s",str); 

	printf("size = %d\n Length - %d\n",sizeof(str),strlen(str));
	return 0;



	
}
