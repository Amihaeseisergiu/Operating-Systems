#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include <time.h>
#include <stdint.h>

#define MAX_NO_WORKERS      24
#define MAX_LENGTH_FILENAME 50

int main(int argc, char* argv[])
{
	int nr=0, k, NW=0, i, pid;
	int64_t sum = 0;
	char  input_filenames[MAX_NO_WORKERS][MAX_LENGTH_FILENAME];
	char output_filenames[MAX_NO_WORKERS][MAX_LENGTH_FILENAME];
	FILE *fdi[MAX_NO_WORKERS], *fdo, *fs;

	struct timespec tic, toc;

        clock_gettime(CLOCK_MONOTONIC_RAW,&tic);


	if(argc < 3)
	{
		printf("Usage:%s no_workers nume_fisier\n",argv[0]);
		exit(0);
	}

	NW = atoi(argv[1]);
	if( NULL == (fs = fopen(argv[2],"r") ) )
	{
		fprintf(stderr,"Supervisior: eroare la deschiderea fisierului de citire %s.\n", argv[2]);
		perror("Cauza este: ");
		exit(5);
	}

	if( (NW < 1) || (NW > MAX_NO_WORKERS) )
	{
		fprintf(stderr,"Date de intrare incorecte.\n");  exit(1);
	}

	for (i = 1; i <= NW; ++i)
	{
		sprintf( input_filenames[i], "f%di.txt", i);
		sprintf( output_filenames[i], "f%do.txt", i);
	}

	for (i = 1; i <= NW; ++i)
	{
		if( NULL == (fdi[i-1] = fopen(input_filenames[i], "w")) )
		{
			fprintf(stderr,"Supervisor : eroare la deschiderea fisierului de intrare %s.\n", input_filenames[i]);
			perror("Cauza erorii: ");
			exit(2);
		}
	}

	k = 0;
	while( !feof(fs) )
	{
		fread(&nr,4,1,fs);
		if(nr != 0 && !feof(fs) )
			fprintf(fdi[k],"%d ",nr);
		k = (k+1) % NW;
	}
	fclose(fs);

	for (i = 1; i <= NW; ++i)
	{
		fclose(fdi[i-1]);
	}

	for (i = 1; i <= NW; ++i)
	{
		if(-1 == (pid = fork()) )
		{
			fprintf(stderr,"Supervisor : eroare la apelul fork() pentru procesul worker al %d-lea.\n", i);
			perror("Cauza erorii: ");
			exit(3);
		}

		if (pid == 0)
		{
			execl("worker", "worker", input_filenames[i], output_filenames[i], NULL);

			fprintf(stderr,"Supervisor : eroare la apelul exec() pentru procesul worker al %d-lea.\n", i);
			perror("Cauza erorii: ");
			exit(4);
		}
	}

	for (i = 1; i <= NW; ++i)
	{
		wait(NULL);
	}

	for (i = 1; i <= NW; ++i)
	{
		if (NULL == (fdo = fopen(output_filenames[i], "r")) )
		{
			fprintf(stderr,"Supervisor : eroare la deschiderea fisierului de iesire %s.\n", output_filenames[i]);
			perror("Cauza erorii: ");
			exit(5);
		}

		if (1 != fscanf(fdo, "%d", &nr) )
		{
			fprintf(stderr,"Supervisor : eroare la citirea celei de a %d-a sume partiale.\n", i);
			exit(6);
		}
		else
			sum += nr;

		fclose(fdo);
	}

	printf("Supervisor: suma secventei introduse este: %I64ld.\n", sum);

	clock_gettime(CLOCK_MONOTONIC_RAW,&toc);

        printf("%f\n",(toc.tv_nsec - tic.tv_nsec) / 1000000000.0 + (toc.tv_sec  - tic.tv_sec));


	for (i = 1; i <= NW; ++i)
	{
		remove( input_filenames[i]);
		remove( output_filenames[i]);
	}
	return 0;
}
