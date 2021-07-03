#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

void merge(int fd, int l1, int h1, int h2);

void bubble_sort(int filedescr, int st, int end);

void mergeSort(int fd, int l, int h)
{
	int len = (h-l+1);

	if (len <= 5)
	{
		//printf("bubble sort de la %d la %d\n",l,l+len);
		bubble_sort(fd, l, l+len);
		return;
	}

	pid_t lpid,rpid;
	lpid = fork();
	if (lpid < 0)
	{
		perror("Left Child Proc. not created\n");
		exit(0);
	}
	else if (lpid == 0)
	{
		mergeSort(fd, l, l+len/2-1);
		exit(1);
	}
	else
	{
		rpid = fork();
		if (rpid < 0)
		{
			perror("Right Child Proc. not created\n");
			exit(0);
		}
		else if(rpid == 0)
		{
			mergeSort(fd, l+len/2, h);
			exit(1);
		}
	}

	int status;

	waitpid(lpid, &status, 0);
	waitpid(rpid, &status, 0);

	//printf("Merge %d-%d cu %d-%d\n",l,l+len/2-1,l+len/2-1,h);
	merge(fd, l, l+len/2-1, h);
}


void merge(int fd, int l1, int h1, int h2)
{
	int fs;
	int i=l1, k=h1+1, pos1, pos2, rcod1, rcod2, nr1, nr2;
	//int count=h2-l1+1;

	if(-1 == (fs = open("lab9tmp.bin", O_RDWR | O_CREAT | O_TRUNC, 0600) ) )
	{
		perror("Eroare la crearea fisierului temporar");  exit(2);
	}

	/*lseek(fd, i*sizeof(int),SEEK_SET);
	printf("Fisier %d-%d: ",l1,h1);
	while(1)
        {
                pos1 = lseek(fd, i*sizeof(int), SEEK_SET)/sizeof(int);
                if(pos1 > h1) break;
                rcod1 = read(fd, &nr1, sizeof(int));
                if(rcod1 == 0) break;
                if(rcod1 == -1)
                {
                        perror("Eroare la citirea din fisier");
                        exit(9);
                }
                i++;
                printf("%d ",nr1);
        }
	i=h1+1;
	lseek(fd, i*sizeof(int),SEEK_SET);
	printf("\nFisier %d-%d: ",h1+1,h2);
        while(1)
        {
                pos1 = lseek(fd, i*sizeof(int), SEEK_SET)/sizeof(int);
                if(pos1 > h2) break;
                rcod1 = read(fd, &nr1, sizeof(int));
                if(rcod1 == 0) break;
                if(rcod1 == -1)
                {
                        perror("Eroare la citirea din fisier");
                        exit(9);
                }
                i++;
                printf("%d ",nr1);
        }
        printf("\n");
        i=l1;
	*/
	while (1)
	{
		pos1 = lseek(fd, i*sizeof(int), SEEK_SET)/sizeof(int);
		//printf("pos1=%d\n",pos1);
		if(pos1 > h1) break;
		rcod1 = read(fd, &nr1, sizeof(int));
		if(rcod1 == 0) break;
		if(rcod1 == -1)
		{
			perror("Eroare la citirea din fisier");
			exit(4);
		}


		pos2 = lseek(fd, k*sizeof(int), SEEK_SET)/sizeof(int);
		//printf("pos2=%d\n",pos2);
		if(pos2 > h2) break;
		rcod2 = read(fd, &nr2, sizeof(int));
		if(rcod2 == 0) break;
                if(rcod2 == -1)
                {
                        perror("Eroare la citirea din fisier");
                        exit(4);
                }

		if (nr1 < nr2)
		{
			//printf("%d < %d\n",nr1,nr2);
			i++;
			if(-1 == write(fs, &nr1, sizeof(int)))
                        {
                        	 perror("Eroare la scrierea in fisierul temporar");  exit(5);
                        }

		}
		else if (nr2 < nr1)
		{
			//printf("%d < %d\n",nr2,nr1);
			k++;
			if(-1 == write(fs, &nr2, sizeof(int)))
                        {
                                 perror("Eroare la scrierea in fisierul temporar");  exit(6);
                        }
		}
		else if (nr1 == nr2)
		{
			//printf("%d == %d\n",nr1,nr2);
			i++;
			k++;
			if(-1 == write(fs, &nr1, sizeof(int)))
                        {
                                 perror("Eroare la scrierea in fisierul temporar");  exit(7);
                        }
			if(-1 == write(fs, &nr2, sizeof(int)))
                        {
                                 perror("Eroare la scrierea in fisierul temporar");  exit(8);
                        }

		}
	}

	while (1)
	{
		pos1 = lseek(fd, i*sizeof(int), SEEK_SET)/sizeof(int);
                if(pos1 > h1) break;
                rcod1 = read(fd, &nr1, sizeof(int));
                if(rcod1 == 0) break;
                if(rcod1 == -1)
                {
                        perror("Eroare la citirea din fisier");
                        exit(9);
                }
		i++;
                if(-1 == write(fs, &nr1, sizeof(int)))
       	        {
                        perror("Eroare la scrierea in fisierul temporar");  exit(10);
                }

	}

	while(1)
	{
		pos2 = lseek(fd, k*sizeof(int), SEEK_SET)/sizeof(int);
                if(pos2 > h2) break;
                rcod2 = read(fd, &nr2, sizeof(int));
                if(rcod2 == 0) break;
                if(rcod2 == -1)
                {
                        perror("Eroare la citirea din fisier");
                        exit(11);
                }
                k++;
                if(-1 == write(fs, &nr2, sizeof(int)))
                {
                        perror("Eroare la scrierea in fisierul temporar");  exit(12);
                }

	}

	lseek(fs, 0L, SEEK_SET);
	i = l1;
	while(1)
	{
		pos1 = lseek(fd, i*sizeof(int), SEEK_SET)/sizeof(int);
		//if(pos1 > count-1){printf("In count\n"); break; }
		rcod1 = read(fs, &nr1, sizeof(int));
                if(rcod1 == 0){break; }
                if(rcod1 == -1)
                {
                        perror("Eroare la citirea din fisier");
                        exit(9);
                }
		i++;
		//printf("%d ",nr1);
		if(-1 == write(fd, &nr1, sizeof(int)))
                {
                        perror("Eroare la scrierea in fisierul temporar");  exit(12);
                }

	}
	//printf("\n");
	close(fs);

	if( -1 == remove("lab9tmp.bin") )
	{
		perror("Eroare la stergerea fisierului de date:");
		exit(3);
	}

}


void bubble_sort(int filedescr, int st, int end)
{
	int rcod1,rcod2;
	int numar1,numar2;
	int numar1_recitit,numar2_recitit;
	int pos;

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

	lseek(filedescr, st*sizeof(int), SEEK_SET);

	while(modificare)
	{
		modificare = 0;

		while(1)
		{
			rcod1 = read(filedescr, &numar1, sizeof(int));
			pos = lseek(filedescr, 0L, SEEK_CUR)/sizeof(int);
			//printf("Am citit %d de la pozitia %d\n",numar1,pos);
			if(pos > end) break;
			if(rcod1 == 0) break;
			if(rcod1 ==-1)
			{
				perror("Eroare la citirea primului numar dintr-o pereche");  exit(3);
			}

			rcod2 = read(filedescr, &numar2, sizeof(int));
			pos = lseek(filedescr, 0L, SEEK_CUR)/sizeof(int);
			//printf("Am citit %d de la pozitia %d\n",numar2,pos);
			if(pos > end) break;
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
			}

			if(-1 == lseek(filedescr, -sizeof(int), SEEK_CUR))
			{
				perror("Eroare (2) la repozitionarea inapoi in fisier");  exit(8);
			}
		}

		if(-1 == lseek(filedescr, st*sizeof(int), SEEK_SET))
		{
			perror("Eroare (3) la repozitionarea inapoi in fisier");  exit(9);
		}

    }
}

int main(int argc, char* argv[])
{

	int fd, size;

	if(argc < 2)
	{
		printf("Usage: %s nume_fisier\n",argv[0]);
		exit(0);
	}

	if( -1 == ( fd = open(argv[1],O_RDWR ) ) )
	{
		printf("Eroare la deschiderea fisierului %s\n",argv[1]);
		exit(1);
	}

	size = lseek(fd,0L,SEEK_END)/sizeof(int);

	mergeSort(fd, 0, size-1);

	close(fd);
	return 0;
}
