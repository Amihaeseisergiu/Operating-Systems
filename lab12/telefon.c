#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>

int main(void)
{
	int fd_in, fd_out, f_telefon, rcod = 1;
	char ch = '1', ch2;

	if(-1 == mkfifo("fifo_nume2telefon", 0600) )
	{
		if(errno != EEXIST)
		{
			perror("Eroare la crearea canalului 'fifo_nume2telefon'. Cauza erorii");  exit(1);
		}
	}
	if(-1 == mkfifo("fifo_telefon2nume", 0600) )
	{
		if(errno != EEXIST)
		{
			perror("Eroare la crearea canalului 'fifo_telefon2nume'. Cauza erorii");  exit(2);
		}
	}

	if(-1 == (f_telefon = open("telefon.txt", O_RDONLY) ) )
	{
		fprintf(stderr,"Eroare la deschiderea fisierului telefon.txt");
		perror("Cauza erorii");
		exit(3);
	}

	fd_in  = open("fifo_nume2telefon",O_RDONLY);
	fd_out = open("fifo_telefon2nume",O_WRONLY);

	while(rcod != 0)
	{
		read(fd_in,&ch,1);

		while(1)
		{
			rcod = read(f_telefon, &ch2, 1);
			if(ch2 == '\n' || rcod == 0) break;
			printf("%c",ch2);
		}

		printf("\n");

		fflush(stdout);

		write(fd_out,&ch,1);
	}

	return 0;
}
