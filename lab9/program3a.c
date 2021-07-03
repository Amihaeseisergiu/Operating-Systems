#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>

int N,k;

int creeaza_fiu(int i);

int main(int argc, char* argv[])
{
	if(argc < 3)
	{
		printf("Introduceti N,k: ");
		while( (1 != scanf("%d %d", &N, &k)) && (N < 1) && (k < 1) ) { fprintf(stderr,"\nEroare: nu ati introdus un numar intreg strict pozitiv! Incercati din nou..."); }
	}
	else
	{
		if(1 != sscanf(argv[1],"%d", &N)) { fprintf(stderr,"Eroare: nu ati specificat un numar intreg strict pozitiv!\n");  exit(1); }
		if(1 != sscanf(argv[2],"%d", &k)) { fprintf(stderr,"Eroare: nu ati specificat un numar intreg strict pozitiv!\n");  exit(1); }
	}

	return creeaza_fiu(1);
}

int creeaza_fiu(int i)
{
	int j;
	pid_t pid;

	if (i < N)
	{
		printf("%d-%d\n",getpid(),getppid());
		fflush(stdout);

		for (j = 0; j < k; j++)
	        {
			pid = fork();
      			if (pid == -1)
			{
        			perror("Eroare la fork()");
        			return 1;
      			}
			else if (pid == 0)
			{
        			return creeaza_fiu(i + 1);
      			}
    		}
		for(j = 0; j < k; j++)wait(NULL);
  	}
	else
	{
		printf("%d-%d\n",getpid(),getppid());
  	}
  	return 0;
}

