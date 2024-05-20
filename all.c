#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    int pid[4];
    char *commands[] = {"gcc -c circle.c", "gcc -c square.c", "gcc -c rectangle.c", "gcc -c main.c"};
    for(int i = 0; i < 4; ++i)
	{
        pid[i] = fork();
        if(pid[i] == 0)
		{
            execlp("sh", "sh", "-c", commands[i], NULL);
            perror("exec");
            exit(1);
        }
		else if(pid[i] < 0)
		{
            perror("fork");
            exit(1);
        }
    }
    int status;
    for(int i = 0; i < 4; ++i)
	{
        waitpid(pid[i], &status, 0);
        if(!WIFEXITED(status) || WEXITSTATUS(status) != 0)
		{
            printf("Child process %d failed\n", pid[i]);
            exit(1);
        }
    }
    int link_pid = fork();
    if(link_pid == 0)
	{
        execlp("sh", "sh", "-c", "gcc -o program.out circle.o square.o rectangle.o main.o", NULL);
        perror("exec");
        exit(1);
    }
	else if(link_pid < 0)
	{
        perror("fork");
        exit(1);
    }
    waitpid(link_pid, &status, 0);
    if(!WIFEXITED(status) || WEXITSTATUS(status) != 0)
	{
        printf("Linking failed\n");
        exit(1);
    }
    int run_pid = fork();
    if(run_pid == 0)
	{
        execl("./program.out", "program.out", NULL);
        perror("exec");
        exit(1);
    }
	else if(run_pid < 0)
	{
        perror("fork");
        exit(1);
    }
    waitpid(run_pid, &status, 0);
    if(!WIFEXITED(status))
	{
        printf("Child process %d terminated abnormally\n", run_pid);
        exit(1);
    }
    printf("Child exit status: %d\n", WEXITSTATUS(status));
    return 0;
}

