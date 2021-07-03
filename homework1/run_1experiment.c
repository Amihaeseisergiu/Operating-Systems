#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char* argv[])
{
	int N, i;
	pid_t pid;

	struct timespec tic, toc;

	clock_gettime(CLOCK_MONOTONIC_RAW,&tic);

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

	clock_gettime(CLOCK_MONOTONIC_RAW,&toc);

	printf("%f\n",(toc.tv_nsec - tic.tv_nsec) / 1000000000.0 + (toc.tv_sec  - tic.tv_sec));

	return 0;
}
