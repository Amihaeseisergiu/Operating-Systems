#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

int main(void)
{
	int fd_in, fd_out, rcod;
	int dot = 0, first = 0;
	char ch;

	if(-1 == mkfifo("canal-extern-1", 0600) )
        {
		if(errno != EEXIST)
                {
			fprintf(stderr,"Eroare la crearea fisierului fifo 'cana-extern-1'");
                	perror("Cauza erorii:");
                	exit(1);
		}
        }

	if(-1 == mkfifo("canal-extern-2", 0600) )
	{
                if(errno != EEXIST)
                {
                        fprintf(stderr,"Eroare la crearea fisierului fifo 'cana-extern-2'");
                        perror("Cauza erorii:");
                        exit(1);
                }
        }

	fd_in = open("canal-extern-1", O_RDONLY);
	fd_out = open("canal-extern-2", O_WRONLY);

	while(1)
	{
		rcod = read(fd_in, &ch, 1);
		if(rcod == 0)break;
		if(first == 0 && (ch >= 'a' && ch <= 'z') )
		{
			first = 1;
			ch = ch - 32;
			write(fd_out, &ch, 1);
		}
		else if(ch >= 'a' && ch <= 'z' && dot == 1)
		{
			dot = 0;
			ch = ch - 32;
			write(fd_out, &ch, 1);
		}
		else if(ch == '.')
		{
			dot = 1;
			write(fd_out, &ch, 1);
		}
		else
		{
			write(fd_out, &ch, 1);
			first = 1;
		}
	}
	close(fd_in);
	close(fd_out);

	return 0;

}
