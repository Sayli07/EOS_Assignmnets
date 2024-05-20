#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>


int main()
{
	int signum, procid;
	printf("Enter signal number: ");
	scanf("%d", &signum);
	printf("Enter process ID: ");
	scanf("%d", &procid);
	int result = kill(procid, signum);
	if(result == 0)
	{
		printf("Signal %d sent to process %d\n",signum, procid);
	}
	else
	{
		perror("Failed to send signal");
				_exit(1);
	}
	return 0;
}
