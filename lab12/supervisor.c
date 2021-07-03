#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

int main(void)
{
	int socket, fisier, rcod, cod;
	char ch;

	if(-1 == (fisier = open("comenzi.sh", O_RDONLY) ) )
	{
		perror("Eroare la deschiderea fisierului 'comenzi.sh':");
		exit(0);
	}

	if(-1 == mkfifo("socket", 0600) )
	{
		if(errno != EEXIST)
		{
			perror("Eroare la deschiderea canalului in supervisor:");
			exit(1);
		}
	}

	while(1)
	{
		socket = open("socket", O_WRONLY);
		while(1)
		{
			rcod = read(fisier, &ch, 1);
			if(rcod == 0 || ch == '\n')
			{
				write(socket, &ch, 1);
				break;
			}
			write(socket, &ch, 1);
		}
		if(rcod == 0)break;
		close(socket);
		socket = open("socket", O_RDONLY);
		read(socket, &cod, 4);
		if(cod == 99)break;
		printf("Codul de terminare : %d\n", cod);
		close(socket);
	}
	close(socket);
	return 0;
}
