#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

void sort_i(int filedescr);

void sort_s(int filedescr);

void sort_a(int filedescr);

int main(int argc,char *argv[])
{
	int fd;
	if(argc < 3)
	{
		printf("Usage: %s datafile argument\n", argv[0]);  exit(1);
	}

	if(-1 == (fd = open(argv[1],O_RDWR)))
	{
		perror("Eroare la deschiderea fisierului de date\n");  exit(2);
	}

	if( strcmp(argv[2],"-i") == 0 ) sort_i(fd);
	else if( strcmp(argv[2],"-s") == 0) sort_s(fd);
	else if( strcmp(argv[2],"-a") == 0) sort_a(fd);

	close(fd);

	fprintf(stderr, "Notificare: [PID: %d] Am terminat (contributia mea la) sortarea fisierului %s !\n", getpid(), argv[1]);
	return 0;
}


void sort_i(int filedescr)
{
	int rcod1,rcod2;
	int numar1,numar2;
	int numar1_recitit,numar2_recitit;

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

	while(modificare)
	{
		modificare = 0;

		while(1)
		{
			rcod1 = read(filedescr, &numar1, sizeof(int));
			if(rcod1 == 0) break;
			if(rcod1 ==-1)
			{
				perror("Eroare la citirea primului numar dintr-o pereche\n");  exit(3);
			}

			rcod2 = read(filedescr, &numar2, sizeof(int));
			if(rcod2 == 0) break;
			if(rcod2 ==-1)
			{
				perror("Eroare la citirea celui de-al doileai numar dintr-o pereche\n");  exit(4);
			}

			if(numar1 > numar2)
			{

				if(-1 == lseek(filedescr, -2*sizeof(int), SEEK_CUR))
				{
					perror("Eroare (1) la repozitionarea inapoi in fisier\n");  exit(5);
				}

				if(-1 == fcntl(filedescr, F_SETLKW, &lacat_blocaj))
				{
					perror("Eroare la blocaj\n");  exit(10);
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

					if(-1 == lseek(filedescr, -2*sizeof(int), SEEK_CUR))
					{
						perror("Eroare (4) la repozitionarea inapoi in fisier\n");  exit(13);
					}

					if(-1 == write(filedescr, &numar2_recitit, sizeof(int)))
					{
						perror("Eroare la rescrierea primului numar dintr-o pereche\n");  exit(14);
					}

					if(-1 == write(filedescr, &numar1_recitit, sizeof(int)))
					{
						perror("Eroare la rescrierea celui de-al doilea numar dintr-o pereche\n");  exit(15);
					}
				}

				if(-1 == fcntl(filedescr, F_SETLKW, &lacat_deblocaj))
				{
					perror("Eroare la deblocaj\n");  exit(16);
				}
			}

			if(-1 == lseek(filedescr, -sizeof(int), SEEK_CUR))
			{
				perror("Eroare (2) la repozitionarea inapoi in fisier\n");  exit(8);
			}
		}

		if(-1 == lseek(filedescr, 0L, SEEK_SET))
		{
			perror("Eroare (3) la repozitionarea inapoi in fisier\n");  exit(9);
		}

    }

}

void sort_s(int filedescr)
{
        int rcod1,rcod2;
        int numar1,numar2;
        int numar1_recitit,numar2_recitit;

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

	if(-1 == lseek(filedescr, 0L, SEEK_END))
	{
		perror("Eroare (1) la repozitionarea la final in fisier\n");
		exit(0);
	}

        while(modificare)
        {
                modificare = 0;

                while(1)
                {
			if(-1 == lseek(filedescr, -2*sizeof(int),SEEK_CUR) )
			{
				break;
			}

                        rcod1 = read(filedescr, &numar1, sizeof(int));

                        if(rcod1 ==-1)
                        {
                                perror("Eroare la citirea primului numar dintr-o pereche\n");  exit(3);
                      	}

                        rcod2 = read(filedescr, &numar2, sizeof(int));

                        if(rcod2 ==-1)
                        {
                                perror("Eroare la citirea celui de-al doileai numar dintr-o pereche\n");  exit(4);
                        }

                        if(numar1 > numar2)
                        {

                                if(-1 == lseek(filedescr, -2*sizeof(int), SEEK_CUR))
                                {
                                        perror("Eroare (1) la repozitionarea inapoi in fisier\n");  exit(5);
                                }

                                if(-1 == fcntl(filedescr, F_SETLKW, &lacat_blocaj))
                                {
                                        perror("Eroare la blocaj\n");  exit(10);
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

                                        if(-1 == lseek(filedescr, -2*sizeof(int), SEEK_CUR))
                                        {
                                                perror("Eroare (4) la repozitionarea inapoi in fisier\n");  exit(13);
                                        }

                                        if(-1 == write(filedescr, &numar2_recitit, sizeof(int)))
                                        {
                                                perror("Eroare la rescrierea primului numar dintr-o pereche\n");  exit(14);
                                        }

                                        if(-1 == write(filedescr, &numar1_recitit, sizeof(int)))
                                        {
                                                perror("Eroare la rescrierea celui de-al doilea numar dintr-o pereche\n");  exit(15);
                                        }
                                }

                                if(-1 == fcntl(filedescr, F_SETLKW, &lacat_deblocaj))
                                {
                                        perror("Eroare la deblocaj\n");  exit(16);
                                }
                        }

                        if(-1 == lseek(filedescr, -sizeof(int), SEEK_CUR))
                        {
				break;
                        }
                }

                if(-1 == lseek(filedescr, 0L, SEEK_END))
                {
                        perror("Eroare (3) la repozitionarea inapoi in fisier");  exit(9);
                }

    }

}

void sort_a(int filedescr)
{
        int rcod1,rcod2;
        int numar1,numar2;
        int numar1_recitit,numar2_recitit;

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

        while(modificare)
        {
                modificare = 0;

                while(1)
                {
                        rcod1 = read(filedescr, &numar1, sizeof(int));
                        if(rcod1 == 0) break;
                        if(rcod1 ==-1)
                        {
                                perror("Eroare la citirea primului numar dintr-o pereche\n");  exit(3);
                        }

                        rcod2 = read(filedescr, &numar2, sizeof(int));
                        if(rcod2 == 0) break;
                        if(rcod2 ==-1)
                        {
                                perror("Eroare la citirea celui de-al doileai numar dintr-o pereche\n");  exit(4);
                        }

                        if(numar1 > numar2)
                        {

                                if(-1 == lseek(filedescr, -2*sizeof(int), SEEK_CUR))
                                {
                                        perror("Eroare (1) la repozitionarea inapoi in fisier\n");  exit(5);
                                }

                                if(-1 == fcntl(filedescr, F_SETLKW, &lacat_blocaj))
                                {
                                        perror("Eroare la blocaj\n");  exit(10);
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

                                        if(-1 == lseek(filedescr, -2*sizeof(int), SEEK_CUR))
                                        {
                                                perror("Eroare (4) la repozitionarea inapoi in fisier\n");  exit(13);
                                        }

                                        if(-1 == write(filedescr, &numar2_recitit, sizeof(int)))
                                        {
                                                perror("Eroare la rescrierea primului numar dintr-o pereche\n");  exit(14);
                                        }

                                        if(-1 == write(filedescr, &numar1_recitit, sizeof(int)))
                                        {
                                                perror("Eroare la rescrierea celui de-al doilea numar dintr-o pereche\n");  exit(15);
                                        }
                                }

                                if(-1 == fcntl(filedescr, F_SETLKW, &lacat_deblocaj))
                                {
                                        perror("Eroare la deblocaj\n");  exit(16);
                                }
                        }

                        if(-1 == lseek(filedescr, -sizeof(int), SEEK_CUR))
                        {
                                perror("Eroare (2) la repozitionarea inapoi in fisier\n");  exit(8);
                        }
                }

		if(modificare == 0) break;

		modificare = 0;

                while(1)
                {
                        if(-1 == lseek(filedescr, -2*sizeof(int),SEEK_CUR) )
                        {
                                break;
                        }

                        rcod1 = read(filedescr, &numar1, sizeof(int));

                        if(rcod1 ==-1)
                        {
                                perror("Eroare la citirea primului numar dintr-o pereche\n");  exit(3);
                        }

                        rcod2 = read(filedescr, &numar2, sizeof(int));

                        if(rcod2 ==-1)
                        {
                                perror("Eroare la citirea celui de-al doileai numar dintr-o pereche\n");  exit(4);
                        }

                        if(numar1 > numar2)
                        {

                                if(-1 == lseek(filedescr, -2*sizeof(int), SEEK_CUR))
                                {
                                        perror("Eroare (1) la repozitionarea inapoi in fisier\n");  exit(5);
                                }

                                if(-1 == fcntl(filedescr, F_SETLKW, &lacat_blocaj))
                                {
                                        perror("Eroare la blocaj\n");  exit(10);
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

                                        if(-1 == lseek(filedescr, -2*sizeof(int), SEEK_CUR))
                                        {
                                                perror("Eroare (4) la repozitionarea inapoi in fisier\n");  exit(13);
                                        }

                                        if(-1 == write(filedescr, &numar2_recitit, sizeof(int)))
                                        {
                                                perror("Eroare la rescrierea primului numar dintr-o pereche\n");  exit(14);
                                        }

                                        if(-1 == write(filedescr, &numar1_recitit, sizeof(int)))
                                        {
                                                perror("Eroare la rescrierea celui de-al doilea numar dintr-o pereche\n");  exit(15);
                                        }
                                }

                                if(-1 == fcntl(filedescr, F_SETLKW, &lacat_deblocaj))
                                {
                                        perror("Eroare la deblocaj\n");  exit(16);
                                }
                        }

                        if(-1 == lseek(filedescr, -sizeof(int), SEEK_CUR))
                        {
                                break;
                        }
                }

    }

}
