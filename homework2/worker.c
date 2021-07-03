#include <stdio.h>

int main(int argc, char *argv[])
{
	int nr, sum=0;
	FILE *fd;

	// Pasul #1: citirea numerelor si calculul sumei

	if (NULL == (fd = fopen(argv[1], "r")) )
	{
		fprintf(stderr,"Worker : eroare la deschiderea fisierului de intrare %s.\n", argv[1]);
		return 1;
	}
	while(! feof(fd) )
	{
		if(1 == fscanf(fd, "%d", &nr) )
			sum += nr;
	}
	fclose(fd);


	// Pasul #2: scrierea sumei calculate 

	if (NULL == (fd = fopen(argv[2], "w")) )
	{
		fprintf(stderr,"Worker : eroare la deschiderea fisierului de iesire %s.\n", argv[2]);
		return 2;
	}
	fprintf(fd, "%d\n", sum);
	fclose(fd);

	return 0;
}
