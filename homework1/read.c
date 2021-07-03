#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char* argv[])
{
	int nr,rcod,fd;

	if(argc < 2)
	{
		printf("Usage:%s nume_fisier\n",argv[0]);
		exit(0);
	}
	if (-1 == (fd = open(argv[1], O_RDONLY)))
	{
		perror("Eroare la deschiderea fisierului de date");  exit(1);
	}

	printf("Secventa de numere din fisierul %s este:\n", argv[1]);
	while(1)
	{
		if (-1 == (rcod = read(fd,&nr,sizeof(int))) )
		{
			perror("Eroare la citirea din fisierul de date");  exit(2);
		}
		if (0 == rcod) break;
		printf("%d  ",nr);
	}
	printf("\n\n");

	close(fd);
	return 0;
}

