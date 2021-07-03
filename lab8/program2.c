#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

void do_shit(int filedescr1,int filedescr2);

int main(int argc,char *argv[])
{
	int fd,fi;
	if(argc < 3)
	{
		printf("Usage: %s fisier de date, %s fisier cu instructiuni\n", argv[0], argv[0]);  exit(1);
	}

	if(-1 == (fd = open(argv[1],O_RDWR)))
	{
		perror("Eroare la deschiderea fisierului de date");  exit(2);
	}

	if(-1 == (fi = open(argv[2],O_RDONLY)))
	{
		perror("Eroare la deschiderea fisierului de instructiuni"); exit(3);
	}

	do_shit(fd,fi);

	close(fd);
	close(fi);

	printf("Notificare: [PID: %d] a terminat contributia la fisierul %s cu instructiunile din %s!\n", getpid(), argv[1], argv[2]);
	sleep(1);

	return 0;
}


void do_shit(int filedescr1,int filedescr2)
{
	int cod_produs1,cod_produs2;
	int cantitate1,cantitate2,cantitate_noua;
	int rcod1,rcod2;
	int gasit;

	struct flock lacat_blocaj, lacat_deblocaj;

	lacat_blocaj.l_type   = F_WRLCK;
	lacat_blocaj.l_whence = SEEK_CUR;
	lacat_blocaj.l_start  = 0;
	lacat_blocaj.l_len    = sizeof(int);

	lacat_deblocaj.l_type   = F_UNLCK;
	lacat_deblocaj.l_whence = SEEK_CUR;
	lacat_deblocaj.l_start  = 0;
	lacat_deblocaj.l_len    = -sizeof(int);

	while(1)
	{

		rcod1 = read(filedescr2, &cod_produs1, sizeof(int));

		if(rcod1 == 0) break;

		if(rcod1 == -1)
		{
			perror("Eroare la citirea codului produsului din fisierul de instructiuni!");
			exit(4);
		}

		rcod2 = read(filedescr2, &cantitate1, sizeof(int));

		if(rcod2 == 0) break;

		if(rcod2 == -1)
		{
			perror("Eroare la citirea cantitatii produsului din fisierul de instructiuni!");
			exit(5);
		}

		if(-1 == lseek(filedescr1, 0L, SEEK_SET))
        	{
                	perror("Eroare (1) la repozitionarea inapoi in fisier");  exit(16);
	        }

		gasit = 0;
		while(1)
		{
			rcod1 = read(filedescr1, &cod_produs2, sizeof(int));
			if(rcod1 == 0) break;
			if(rcod1 ==-1)
			{
				perror("Eroare la citirea codului produsului din fisier");  exit(6);
			}
			if(-1 == fcntl(filedescr1, F_SETLKW, &lacat_blocaj))
                                {
                                        perror("Eroare la blocaj");  exit(10);
                                }

			rcod2 = read(filedescr1, &cantitate2, sizeof(int));
                        if(rcod2 == 0) break;
                        if(rcod2 ==-1)
                                {
                                perror("Eroare la citirea cantitatii produsului din fisier");  exit(7);
                        }

			if(cod_produs1 == cod_produs2)
			{
				gasit = 1;
				if(-1 == lseek(filedescr1, -sizeof(int), SEEK_CUR))
				{
					perror("Eroare (3) la repozitionarea inapoi in fisier");  exit(13);
				}

				cantitate_noua = cantitate2 + cantitate1;

				if(-1 == write(filedescr1, &cantitate_noua, sizeof(int)))
				{
					perror("Eroare la schimbarea cantitatii");  exit(14);
				}
				printf("Notificare: [PID: %d] a actualizat cheia [%d]: %d -> %d\n",getpid(),cod_produs2,cantitate2,cantitate_noua);
				sleep(1);
			}
			if(-1 == fcntl(filedescr1, F_SETLKW, &lacat_deblocaj))
                        {
                                perror("Eroare la deblocaj");  exit(16);
                        }
		}

		if(gasit == 0)
		{
			if(-1 == write(filedescr1, &cod_produs1, sizeof(int)))
			{
				perror("Eroare la adaugarea unei noi chei!");
				exit(17);
			}
			if(-1 == write(filedescr1, &cantitate1, sizeof(int)))
			{
				perror("Eroare la adaugarea unei noi chei!");
				exit(15);
			}
			printf("Notificare: [PID: %d] a adaugat o cheie noua: [%d] cu valoarea [%d]\n",getpid(),cod_produs1,cantitate1);
			sleep(1);
		}
    }

}
