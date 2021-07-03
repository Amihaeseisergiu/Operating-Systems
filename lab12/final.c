#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

int main(void)
{
	int fd_in, fisier2;
	FILE* fisier1;
	int first = 0, rcod, nrchar = 0, nrpar = 0, enter = 0;
	char ch, ch2 = '\t';
	if(-1 == mkfifo("canal-extern-2", 0600) )
	{
                if(errno != EEXIST)
                {
                        fprintf(stderr,"Eroare la crearea fisierului fifo 'cana-extern-2'");
                        perror("Cauza erorii:");
                        exit(1);
                }
        }

	if(NULL == (fisier1 = fopen("statistica.txt","w+") ) )
	{
		fprintf(stderr,"Eroare la crearea fisierului 'statistica.txt'");
                perror("Cauza erorii:");
                exit(2);
	}

	if(-1 == (fisier2 = open("text_corectat.txt",O_WRONLY | O_CREAT | O_TRUNC,0600) ) )
        {
                fprintf(stderr,"Eroare la crearea fisierului 'text_corectat.txt'");
                perror("Cauza erorii:");
                exit(3);
        }

	fd_in = open("canal-extern-2", O_RDONLY);

	while(1)
	{
		rcod = read(fd_in, &ch, 1);
		if(rcod == 0)break;
		if(first == 0 && ch != '\t')
		{
			first = 1;
			write(fisier2, &ch2, 1);
			write(fisier2, &ch, 1);
		}
		else if(ch == '\n')
		{
			enter = 1;
			nrpar++;
			write(fisier2, &ch, 1);
		}
		else if(ch != '\t' && enter == 1)
		{
			enter = 0;
			write(fisier2, &ch2, 1);
			write(fisier2, &ch, 1);
		}
		else
		{
			first = 1;
			nrchar++;
			write(fisier2, &ch, 1);
		}
	}
	close(fd_in);
	close(fisier2);

	fprintf(fisier1, "Numar de caractere: %d\nNumar de paragrafe: %d", nrchar, nrpar);

	fclose(fisier1);

	return 0;
}
