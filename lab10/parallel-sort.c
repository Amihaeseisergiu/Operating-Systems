#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
	int N, i;
	pid_t pid;

	if(argc < 3)
	{
		printf("Usage: %s GP nume_fisier\n",argv[0]);
		exit(0);
	}

	N = atoi(argv[1]);

	for(i = 1; i <= N; i++)
	{
		if(-1 == (pid=fork()) )
		{
			perror("Eroare la fork");  exit(2);
		}
		if(0 == pid)
		{
			if(-1 == execl("parallel-with-locks","parallel-with-locks",argv[2],NULL) )
			{
				perror("Eroare la exec:");
				exit(12);
			}
			return i;
		}
	}

	for(i = 1; i<= N; i++)
		wait(NULL);

	return 0;
}
