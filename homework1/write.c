#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/time.h>

int main(int argc, char* argv[])
{
    int i,j,n,n2,nr,fd;
    char nume[100];
    struct timeval tv;
    gettimeofday(&tv,NULL);
    srandom(tv.tv_sec);

    if(argc < 3)
	{
	    printf("Usage:%s dimensiune_secventa dimensiune_serii\n",argv[0]);
        exit(0);
	}

    n = atoi(argv[1]);
    n2 = atoi(argv[2]);

    strcpy(nume,"file_");
    strcat(nume,argv[1]);
    strcat(nume,"_best-case.dat");

	if (-1 == (fd = creat(nume,0600)))
	{
		printf("Eroare la crearea fisierului %s\n",nume);  exit(1);
	}

	for(i = 1; i <= n; i++)
	{
		if(-1 == write(fd,&i,sizeof(int)) )
		{
			perror("Eroare la scrierea in fisierul de date");  exit(2);
		}
	}

	close(fd);

    strcpy(nume,"file_");
    strcat(nume,argv[1]);
    strcat(nume,"_worst-case.dat");

	if (-1 == (fd = creat(nume,0600)))
	{
		printf("Eroare la crearea fisierului %s",nume);  exit(1);
	}

	for(i = n; i >= 1; i--)
	{
		if(-1 == write(fd,&i,sizeof(int)) )
		{
			perror("Eroare la scrierea in worst-case");  exit(2);
		}
	}

	close(fd);

    for(i = 1; i <= n2; i++)
    {
        strcpy(nume,"file_");
        strcat(nume,argv[1]);
        char *str = malloc(16);
        sprintf(str,"%d",i);
        strcat(nume,"_random-");
        strcat(nume,str);
        strcat(nume,".dat");

	    if (-1 == (fd = creat(nume,0600)))
	    {
		    perror("Eroare la crearea fisierului random");  exit(1);
	    }

	    for(j = 1; j <= n; j++)
	    {
            nr = random();

		    if(-1 == write(fd,&nr,sizeof(int)) )
		    {
			    perror("Eroare la scrierea in fisierul random");  exit(2);
		    }
	    }

        close(fd);
    }

	return 0;
}

