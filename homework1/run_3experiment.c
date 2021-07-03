#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char* argv[])
{
	pid_t pid;

	struct timespec tic, toc;

	clock_gettime(CLOCK_MONOTONIC_RAW,&tic);

	if(argc < 2)
	{
		printf("Usage: %s nume_fisier\n",argv[0]);
		exit(0);
	}

	if(-1 == (pid=fork()) )
	{
		perror("Eroare la fork");  exit(2);
	}
	if(0 == pid)
	{
		if(-1 == execl("parallel-with-fork","parallel-with-fork",argv[1],NULL) )
		{
			perror("Eroare la exec:");
			exit(12);
		}
		return 1;
	}

	wait(NULL);

	clock_gettime(CLOCK_MONOTONIC_RAW,&toc);

	printf("%f\n",(toc.tv_nsec - tic.tv_nsec) / 1000000000.0 + (toc.tv_sec  - tic.tv_sec));

	return 0;
}
