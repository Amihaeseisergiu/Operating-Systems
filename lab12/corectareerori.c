#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>

int main(void)
{
	int fisier, fd_out;
	int space = 0, semn = 0, rcod;
	char ch, ch2 = ' ';
	if(-1 == (fisier = open("text.txt", O_RDONLY) ) )
	{
		fprintf(stderr,"Eroare la deschiederea fisierului text.txt");
		perror("Cauza erorii:");
		exit(0);
	}
	if(-1 == mkfifo("canal-extern-1", 0600) )
	{
		if(errno != EEXIST)
                {
			fprintf(stderr,"Eroare la crearea fisierului fifo 'cana-extern-1'");
                	perror("Cauza erorii:");
                	exit(1);
		}
        }

	fd_out = open("canal-extern-1", O_WRONLY);

	while(1)
	{
		rcod = read(fisier, &ch, 1);
		if(rcod == 0)break;
		if(ch == ',' || ch == '.' || ch == '?' || ch == '!')
		{
			space = 0;
			semn = 1;
			write(fd_out, &ch, 1);
		}
		else if(ch == ' ' && space == 0)
		{
			space = 1;
			write(fd_out, &ch, 1);
		}
		else if(ch != ' ' && semn == 1)
		{
			semn = 0;
			write(fd_out, &ch2, 1);
			write(fd_out, &ch, 1);
		}
		else if(ch != ' ')write(fd_out, &ch, 1);

	}

	close(fd_out);
	close(fisier);

	return 0;
}
