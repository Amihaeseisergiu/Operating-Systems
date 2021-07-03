#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include <time.h>
#include <stdint.h>

int main(int argc, char* argv[])
{
	int nr = 0;
	int64_t sum = 0;
	FILE *fs;

	struct timespec tic, toc;

        clock_gettime(CLOCK_MONOTONIC_RAW,&tic);

	if(argc < 2)
	{
		printf("Usage:%s nume_fisier\n",argv[0]);
		exit(0);
	}

	if( NULL == (fs = fopen(argv[1],"r") ) )
	{
		fprintf(stderr,"Supervisior: eroare la deschiderea fisierului de citire %s.\n", argv[2]);
		perror("Cauza este: ");
		exit(5);
	}

	while( !feof(fs) )
	{
		fread(&nr,4,1,fs);
		if(nr != 0 && !feof(fs) )
			sum += nr;
	}
	fclose(fs);

	printf("Supervisor: suma secventei introduse este: %I64ld.\n", sum);

	clock_gettime(CLOCK_MONOTONIC_RAW,&toc);

        printf("%f\n",(toc.tv_nsec - tic.tv_nsec) / 1000000000.0 + (toc.tv_sec  - tic.tv_sec));

	return 0;
}
