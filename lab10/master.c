#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

int main(int argc, char* argv[])
{
	pid_t pid;
	int cod_term;
	char semn[1]="s", nr1[16], nr2[16];

	while(1)
	{
		printf("Master>");
		scanf("%s",nr1);

		if(strcmp(nr1,"exit") == 0)break;

		scanf("%s%s",nr2,semn);
		printf("\n");

		if(-1 == (pid=fork()) )
		{
			perror("Eroare la fork"); exit(1);
		}

		if (pid == 0)
		{

			execl("slave","slave",nr1,nr2,semn,NULL);

			perror("Eroare la exec");
			exit(10);
		}


		wait(&cod_term);
		if(WIFEXITED(cod_term) )
			printf("Rezultatul este %d\n",WEXITSTATUS(cod_term) );
	}
	return 0;
}
