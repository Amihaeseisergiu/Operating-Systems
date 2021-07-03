#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>

void dialog_tata()
{
	int fd, ff, rcod;
	char ch;

        if(-1 == (fd = open("replici-parinte.txt",O_RDONLY)))
        {
                perror("Eroare la deschiderea fisierului replici-fiu");  exit(0);
        }

        if(-1 == (ff = open("flag.bin",O_RDWR)))
        {
                perror("Eroare la deschiderea fisierului flag");  exit(1);
        }

	while (1)
	{
		ch = '1';
		printf("\n[P0] Procesul tata, cu PID-ul: %d.\n", getpid());

		while(ch != '\n')
		{
			rcod = read(fd,&ch,1);
			if(rcod == -1)
			{
				perror("Eroare la citire!");
				exit(2);
			}
			if(rcod == 0) break;
			printf("%c",ch);
		}
		printf("\n");
		if(rcod == 0) break;

		ch = '1';
		lseek(ff,0L,SEEK_SET);
	        write(ff,&ch,1);
		lseek(ff,0,SEEK_SET);

		while(ch != '0' && ch != '2')
		{
			rcod = read(ff,&ch,1);
			if(rcod == -1)
			{
				perror("Eroare (2) la citire!");
				exit(3);
			}
			if(rcod == 0)lseek(ff,0L,SEEK_SET);
		}
    	}

	ch = '2';
	lseek(ff,0L,SEEK_SET);
        write(ff,&ch,1);

    	close(fd);
	close(ff);
}

void dialog_fiu()
{
	int fd, ff, rcod;
        char ch;

        if(-1 == (fd = open("replici-fiu.txt",O_RDONLY)))
        {
                perror("Eroare la deschiderea fisierului replici-fiu");  exit(0);
        }

        if(-1 == (ff = open("flag.bin",O_RDWR)))
        {
                perror("Eroare la deschiderea fisierului flag");  exit(1);
        }

        while (1)
        {
		ch = '0';
		while(ch != '1' && ch != '2')
                {
                        rcod = read(ff,&ch,1);
                        if(rcod == -1)
                        {
                                perror("Eroare (2) la citire!");
                                exit(3);
                        }
                        if(rcod == 0)lseek(ff,0L,SEEK_SET);
                }

		lseek(ff,0L,SEEK_SET);
		printf("\n[P1] Procesul fiu, cu PID-ul: %d.\n", getpid());

                while(ch != '\n')
                {
                        rcod = read(fd,&ch,1);
                        if(rcod == -1)
                        {
                                perror("Eroare la citire!");
                                exit(2);
                        }
                        if(rcod == 0) break;
                        printf("%c",ch);
                }
		printf("\n");
                if(rcod == 0) break;

		lseek(ff,0L,SEEK_SET);
		ch = '0';
		write(ff,&ch,1);
        }

	lseek(ff,0L,SEEK_SET);
        ch = '2';
	write(ff,&ch,1);

        close(fd);
        close(ff);

}

int main()
{
	pid_t pid_fiu;

	/* crearea unui proces fiu */
	if(-1 == (pid_fiu=fork()) )
	{
		perror("Eroare la fork");  return 1;
	}

	/* ramificarea executiei in cele doua procese, tata si fiu */
	if(pid_fiu == 0)
	{ /* zona de cod executata doar de catre fiu */
		dialog_fiu();
	}
	else
	{ /* zona de cod executata doar de catre parinte */
		dialog_tata();
	}

	/* zona de cod comuna, executata de catre ambele procese */
	printf("Sfarsitul executiei procesului %s.\n\n", pid_fiu == 0 ? "fiu" : "parinte" );
	return 0;
}
