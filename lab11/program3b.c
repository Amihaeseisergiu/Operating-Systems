#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>

int main()
{
	pid_t pid1, pid2;
	int p[2], q[2];

	if(-1 == pipe(p) )
	{
		fprintf(stderr, "Eroare: nu pot crea primul canal anonim, errno=%d\n",errno);  perror("Cauza erorii");  exit(1);
	}

	if(-1 == (pid1=fork()) )
	{
		perror("Eroare: nu am putut crea primul fiu. Cauza erorii");  exit(2);
	}

	if(pid1 == 0)
	{
		if(-1 == pipe(q) )
		{
			fprintf(stderr, "Eroare: nu pot crea al doilea canal anonim, errno=%d\n",errno);  perror("Cauza erorii");  exit(3);
		}

		if(-1 == (pid2=fork()) )
		{
			perror("Eroare: nu am putut crea al doilea proces fiu. Cauza erorii");  exit(4);
		}

		if(pid2 == 0)
		{
			if(-1 == dup2(q[1],1) )
			{
				perror("Eroare: redirectarea iesirii standard in al doilea proces fiu a esuat. Cauza erorii");  exit(5);
			}

			execlp("w","w","-h",(char *)NULL);
			perror("Eroare la execlp() in al doilea proces fiu. Cauza erorii");
			exit(6);
		}
		else
		{
			close(q[1]);

			if(-1 == dup2(q[0],0) )
			{
				perror("Eroare: redirectarea intrarii standard in primul fiu a esuat. Cauza erorii"); exit(7);
			}

			if(-1 == dup2(p[1],1) )
			{
				perror("Eroare: redirectarea iesirii standard in primul fiu a esuat. Cauza erorii");  exit(8);
			}

			execlp("tr","tr","-s","\" \"",(char *)NULL);
			perror("Eroare la execlp() in primul fiu. Cauza erorii");  exit(9);
		}

	}
	else
	{
		close(p[1]);

		if(-1 == dup2(p[0],0) )
		{
			perror("Eroare: redirectarea intrarii standard in parinte a esuat. Cauza erorii");  exit(10);
		}

		execlp("cut","cut","-d ","-f1,8",(char *)NULL);
		perror("Eroare la execlp() in parinte. Cauza erorii");  exit(11);
	}

	return 0;
}
