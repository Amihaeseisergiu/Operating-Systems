#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>

#define MAX 10

void master_init();
void master_work();
void slave_work(int fdi, int fdo, int who);

int pipei[MAX][2], pipeo[2], nrwork;

int main(int argc, char* argv[])
{
	int pid, i, j;

	if(argc >= 2)
	{
		nrwork = atoi(argv[1]);
		if(nrwork > MAX)nrwork = MAX;
	}
	else nrwork = MAX;
  	printf("\n\n\n\n\n");

  	master_init();

	for(i = 0; i < nrwork; i++)
	{
		if( -1 == ( pid = fork() ) )
		{
			fprintf(stderr,"Error fork la fiul %d\n",i+1);
			perror("Cauza este:");
			exit(2);
		}

		if(pid == 0)
		{
			close(pipeo[0]);
			for(j = 0; j < nrwork; j++)
			{
				if(j == i)close(pipei[i][1]);
				else
				{
					close(pipei[j][0]);
					close(pipei[j][1]);
				}
			}
			slave_work(pipei[i][0],pipeo[1],i+1);
			return i;
		}
	}

	for(j = 0; j < nrwork; j++)
		close(pipei[j][0]);
    	close(pipeo[1]);

  	master_work();
  	return 0;
}

void master_init()
{
	int i;
	for(i = 0; i < nrwork; i++)
	{
		if( pipe( pipei[i] ) == -1 )
		{
			fprintf(stderr,"Eroare la creare pipe-ului %d",i);
			perror("Cauza este:");
			exit(1);
		}
	}

	if( pipe(pipeo) == -1 )
        {
     		perror("Eroare la crearea pipe-ului pipeo");
  	   	exit(1);
  	}
}

void master_work()
{
  	int nr, flag;
  	int sump, total = 0;
	int j;

  	printf("Introduceti numerele (0 pentru terminare):\n");
  	flag=1;
  	while(1)
  	{
    		scanf("%d", &nr);
    		if(nr == 0) break;

		flag = (flag + 1) % nrwork;

    		write( pipei[flag][1], &nr, sizeof(int));
  	}

	for(j = 0; j < nrwork; j++)
		close(pipei[j][1]);


	while( read(pipeo[0], &sump, sizeof(int) ) )
		total += sump;

  	close(pipeo[0]);

  	printf("Master[PID:%d]> Suma secventei de numere introduse este: %d\n", getpid(), total);
}

void slave_work(int fdi, int fdo, int who)
{
  	int nr, cod_r, suma_partiala;

  	suma_partiala=0;

  	do
  	{
    		cod_r = read(fdi, &nr, sizeof(int));
    		switch(cod_r)
    		{
       			case sizeof(int) : suma_partiala += nr ;  break;
       			case 0  : break;
       			default : fprintf(stderr,"Eroare la citirea din canalul pipei %d\n", who);
                 		  exit(3);
  	  	}
  	}while(cod_r != 0);

  	close(fdi);

  	if( write(fdo, &suma_partiala, sizeof(int)) == -1)
  	{
    		perror("Eroare la scrierea in canalul pipeo");
    		exit(4);
  	}

  	close(fdo);

  	printf("Slave %d[PID:%d]> Suma partiala: %d\n", who, getpid(), suma_partiala);
}

