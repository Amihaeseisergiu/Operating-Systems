#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>

void descrescator(int filedescr);

void crescator(int filedescr);

int sortat = 0, total = 0, curent = 1;

int main(int argc,char *argv[])
{
	int fd, i, j, option;

	srandom(getpid());
        option = getpid() % 2;

	if(argc < 2)
	{
		printf("Usage: %s datafile\n", argv[0]);  exit(1);
	}

	for(i = 1; i < argc; i++)
	{
		if( access(argv[i],F_OK) != 0 )
		{
			if( -1 == (fd = open(argv[i],O_RDWR | O_CREAT | O_TRUNC, 0600 )) )
			{
				perror("Eroare (1) la deschiderea fisierului de date"); exit(0);
			}
			int nr, nr2;
			nr = random() % 20 + 1;
			for(j = 1; j <= nr; j++)
			{
				nr2 = random() % 20 + 1;
				if(-1 == write(fd, &nr2, sizeof(int)) )
				{
					perror("Eroare (1) la scrierea in fisier"); exit(1);
				}
			}
			close(fd);
		}
	}

	for(i = 1; i < argc - 1; i++)
	{
		for(j = i+1; j < argc; j++)
		{
			if(strcmp(argv[i],argv[j]) == 1)
			{
				char aux[strlen(argv[i])];
				strcpy(aux,argv[i]);
				strcpy(argv[i],argv[j]);
				strcpy(argv[j],aux);
			}
		}
	}

	curent = (getpid() % (argc-1)) +1;

	if(-1 == (fd = open(argv[curent],O_RDWR)))
        {
                        perror("Eroare la deschiderea fisierului de date");  exit(2);
        }

	while(sortat == 0 && total < 50)
	{

		if(option == 0)printf("Notificare: [PID: %d][Puncte: %d] Sortez crescator in fisierul %s !\n",getpid(), total, argv[curent]);
		else if(option == 1)printf("Notificare: [PID: %d][Puncte: %d] Sortez descrescator in fisierul %s !\n",getpid(), total, argv[curent]);

		if(option == 0)crescator(fd);
		else if(option == 1)descrescator(fd);

		curent++;

		close(fd);

		if(curent >= argc) curent = 1;

                if(-1 == (fd = open(argv[curent],O_RDWR)))
                {
                        perror("Eroare la deschiderea fisierului de date");  exit(2);
                }

		sleep(1);
	}

	close(fd);

	if(sortat == 1)
	{
		if(option == 0)printf("[---xx---]: [PID: %d][Puncte: %d] Am terminat de sortat un fisier crescator %s !\n", getpid(), total, argv[(curent-1)>=1?(curent-1):(argc-1)]);
		else if(option == 1)printf("[---xx---]: [PID: %d][Puncte: %d] Am terminat de sortat un fisier descrescator %s !\n", getpid(), total, argv[(curent-1)>=1?(curent-1):(argc-1)]);

		printf("Continutul fisierului %s : ",argv[(curent-1)>=1?(curent-1):(argc-1)]);

		if(-1 == (fd = open(argv[(curent-1)>=1?(curent-1):(argc-1)],O_RDONLY)))
		{
			perror("Eroare la deschiderea fisierului de date"); exit(3);
		}

		int rcod, nr;

		while(1)
		{
			if (-1 == (rcod = read(fd,&nr,sizeof(int))) )
			{
				perror("Eroare la citirea din fisierul de date");  exit(4);
			}
			if (0 == rcod) break;
			printf("%d  ",nr);
		}
		printf("\n");

		close(fd);
	}
	else if(total >= 50) printf("[---xx---]: [PID: %d][Puncte: %d] Am ajuns la 50 de puncte !\n", getpid(),total);
	printf("\n");
	return 0;
}

void crescator(int filedescr)
{
	int rcod1,rcod2;
	int numar1,numar2;
	int numar1_recitit,numar2_recitit;
	int partial = 0;

	struct flock lacat_blocaj, lacat_deblocaj;

	lacat_blocaj.l_type   = F_WRLCK;
	lacat_blocaj.l_whence = SEEK_CUR;
	lacat_blocaj.l_start  = 0;
	lacat_blocaj.l_len    = 2*sizeof(int);

	lacat_deblocaj.l_type   = F_UNLCK;
	lacat_deblocaj.l_whence = SEEK_CUR;
	lacat_deblocaj.l_start  = -2*sizeof(int);
	lacat_deblocaj.l_len    = 2*sizeof(int);

	int modificare = 1;

	while(modificare && partial < 5)
	{
		modificare = 0;

		while(1)
		{
			rcod1 = read(filedescr, &numar1, sizeof(int));
			if(rcod1 == 0) break;
			if(rcod1 ==-1)
			{
				perror("Eroare la citirea primului numar dintr-o pereche");  exit(3);
			}

			rcod2 = read(filedescr, &numar2, sizeof(int));
			if(rcod2 == 0) break;
			if(rcod2 ==-1)
			{
				perror("Eroare la citirea celui de-al doileai numar dintr-o pereche");  exit(4);
			}

			if(numar1 > numar2)
			{

				if(-1 == lseek(filedescr, -2*sizeof(int), SEEK_CUR))
				{
					perror("Eroare (1) la repozitionarea inapoi in fisier");  exit(5);
				}

				if(-1 == fcntl(filedescr, F_SETLKW, &lacat_blocaj))
				{
					perror("Eroare la blocaj");  exit(10);
				}

				if(-1 == read(filedescr, &numar1_recitit, sizeof(int)))
				{
					perror("Eroare la recitirea primului numar dintr-o pereche");  exit(11);
				}

				if(-1 == read(filedescr, &numar2_recitit, sizeof(int)))
				{
					perror("Eroare la recitirea celui de-al doilea numar dintr-o pereche");  exit(12);
				}

				if(numar1_recitit > numar2_recitit)
				{
					modificare = 1;
					partial++;
					total++;

					if(-1 == lseek(filedescr, -2*sizeof(int), SEEK_CUR))
					{
						perror("Eroare (4) la repozitionarea inapoi in fisier");  exit(13);
					}

					if(-1 == write(filedescr, &numar2_recitit, sizeof(int)))
					{
						perror("Eroare la rescrierea primului numar dintr-o pereche");  exit(14);
					}

					if(-1 == write(filedescr, &numar1_recitit, sizeof(int)))
					{
						perror("Eroare la rescrierea celui de-al doilea numar dintr-o pereche");  exit(15);
					}
				}

				if(-1 == fcntl(filedescr, F_SETLKW, &lacat_deblocaj))
				{
					perror("Eroare la deblocaj");  exit(16);
				}
				if(partial >= 5)
				{
					printf("Notificare: [PID: %d][Puncte: %d] Am ajuns la 5 puncte.Schimb fisierul !\n",getpid(),total);
					sleep(1);
					break;
				}
			}

			if(-1 == lseek(filedescr, -sizeof(int), SEEK_CUR))
			{
				perror("Eroare (2) la repozitionarea inapoi in fisier");  exit(8);
			}
		}

		if(-1 == lseek(filedescr, 0L, SEEK_SET))
		{
			perror("Eroare (3) la repozitionarea inapoi in fisier");  exit(9);
		}

    }
	if(modificare == 0) sortat = 1;

}

void descrescator(int filedescr)
{
        int rcod1,rcod2;
        int numar1,numar2;
        int numar1_recitit,numar2_recitit;
        int partial = 0;

        struct flock lacat_blocaj, lacat_deblocaj;

        lacat_blocaj.l_type   = F_WRLCK;
        lacat_blocaj.l_whence = SEEK_CUR;
        lacat_blocaj.l_start  = 0;
        lacat_blocaj.l_len    = 2*sizeof(int);

        lacat_deblocaj.l_type   = F_UNLCK;
        lacat_deblocaj.l_whence = SEEK_CUR;
        lacat_deblocaj.l_start  = -2*sizeof(int);
        lacat_deblocaj.l_len    = 2*sizeof(int);

        int modificare = 1;

        while(modificare && partial < 5)
        {
                modificare = 0;

                while(1)
                {
                        rcod1 = read(filedescr, &numar1, sizeof(int));
                        if(rcod1 == 0) break;
                        if(rcod1 ==-1)
                        {
                                perror("Eroare la citirea primului numar dintr-o pereche");  exit(3);
                        }

                        rcod2 = read(filedescr, &numar2, sizeof(int));
                        if(rcod2 == 0) break;
                        if(rcod2 ==-1)
                        {
                                perror("Eroare la citirea celui de-al doileai numar dintr-o pereche");  exit(4);
                        }

                        if(numar1 < numar2)
                        {

                                if(-1 == lseek(filedescr, -2*sizeof(int), SEEK_CUR))
                                {
                                        perror("Eroare (1) la repozitionarea inapoi in fisier");  exit(5);
                                }

                                if(-1 == fcntl(filedescr, F_SETLKW, &lacat_blocaj))
                                {
                                        perror("Eroare la blocaj");  exit(10);
                                }

                                if(-1 == read(filedescr, &numar1_recitit, sizeof(int)))
                                {
                                        perror("Eroare la recitirea primului numar dintr-o pereche");  exit(11);
                                }

                                if(-1 == read(filedescr, &numar2_recitit, sizeof(int)))
                                {
                                        perror("Eroare la recitirea celui de-al doilea numar dintr-o pereche");  exit(12);
                                }

                                if(numar1_recitit < numar2_recitit)
                                {
                                        modificare = 1;
					partial++;
					total++;

                                        if(-1 == lseek(filedescr, -2*sizeof(int), SEEK_CUR))
                                        {
                                                perror("Eroare (4) la repozitionarea inapoi in fisier");  exit(13);
                                        }

                                        if(-1 == write(filedescr, &numar2_recitit, sizeof(int)))
                                        {
                                                perror("Eroare la rescrierea primului numar dintr-o pereche");  exit(14);
                                        }

                                        if(-1 == write(filedescr, &numar1_recitit, sizeof(int)))
                                        {
                                                perror("Eroare la rescrierea celui de-al doilea numar dintr-o pereche");  exit(15);
                                        }
                                }

                                if(-1 == fcntl(filedescr, F_SETLKW, &lacat_deblocaj))
                                {
                                        perror("Eroare la deblocaj");  exit(16);
                                }
                                if(partial >= 5)
				{
					printf("Notificare: [PID: %d][Puncte: %d] Am ajuns la 5 puncte.Schimb fisierul !\n",getpid(),total);
					sleep(1);
					break;
				}
                        }

                        if(-1 == lseek(filedescr, -sizeof(int), SEEK_CUR))
                        {
                                perror("Eroare (2) la repozitionarea inapoi in fisier");  exit(8);
                        }
                }

                if(-1 == lseek(filedescr, 0L, SEEK_SET))
                {
                        perror("Eroare (3) la repozitionarea inapoi in fisier");  exit(9);
                }

    }
        if(modificare == 0) sortat = 1;

}
