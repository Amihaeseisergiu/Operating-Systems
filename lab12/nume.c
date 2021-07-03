#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>

int main(void)
{
	int fd_in, fd_out, f_nume, rcod = 1;
	char ch = '0', ch2;

	if(-1 == mkfifo("fifo_nume2telefon", 0600) )
	{
		if(errno != EEXIST)
		{
			perror("Eroare la crearea canalului 'fifo_nume2telefon'. Cauza erorii");  exit(1);
		}
	}

	if(-1 == ( f_nume = open("nume.txt", O_RDONLY) ) )
	{
		fprintf(stderr,"Eroare la deschiderea fisierului 'nume.txt'.");
		perror("Cauza erorii:");
		exit(2);
	}

	fd_out = open("fifo_nume2telefon",O_WRONLY);
	fd_in  = open("fifo_telefon2nume",O_RDONLY);

	while(rcod != 0)
	{

		while(1)
		{
			rcod = read(f_nume, &ch2, 1);
			if(ch2 == '\n' || rcod == 0) break;
			printf("%c",ch2);
		}
		printf(" ");

		fflush(stdout);

		write(fd_out,&ch,1);

		read(fd_in,&ch,1);
	}

	return 0;
}

