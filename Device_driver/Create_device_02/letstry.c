#include<stdio.h>

int main()
{
	int i;
	char str[3][10];
	printf("Enter the data\n");
	for( i = 0 ; i < 3 ;i++)
	{
	scanf("%s",str[i]);
	}
	printf("%s\n",str[0]);

}
