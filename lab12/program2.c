#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

#define MAX 10

void master_init();
void master_work();
void slave_work(int fdi, int fdo, int who);

int fifoi[MAX][2], fifoo[2], nrwork;

int main(int argc, char* argv[])
{
        int pid, i;
	int fd_in, fd_out;
	char nume[100], buffer[10];

        if(argc >= 2)
        {
                nrwork = atoi(argv[1]);
                if(nrwork > MAX)nrwork = MAX;
        }
        else nrwork = MAX;
        printf("\n\n\n\n\n");

        master_init();

        for(i = 0; i < nrwork; i++)
        {
                if( -1 == ( pid = fork() ) )
                {
                        fprintf(stderr,"Error fork la fiul %d\n",i+1);
                        perror("Cauza este:");
                        exit(2);
                }

                if(pid == 0)
                {
			strcpy(nume,"canal-extern-");
			snprintf(buffer, sizeof(buffer), "%d", i);
			strcat(nume, buffer);

			fd_in = open(nume, O_RDONLY);
			fd_out = open("canal-extern-out", O_WRONLY);

			slave_work(fd_in, fd_out ,i+1);
			close(fd_in);
			close(fd_out);
                        return i;
                }
        }

        master_work();

	for(i = 0; i < nrwork; i++)
	{
		strcpy(nume,"canal-extern-");
		snprintf(buffer,sizeof(buffer), "%d", i);
		strcat(nume, buffer);
		remove(nume);
	}
	remove("canal-extern-out");

        return 0;
}

void master_init()
{
        int i;
	char nume[100], buffer[10];
        for(i = 0; i < nrwork; i++)
        {
		strcpy(nume,"canal-extern-");
		snprintf(buffer, sizeof(buffer), "%d", i);
		strcat(nume, buffer);
                if( mkfifo(nume, 0666) == -1 )
                {
                        if(errno == EEXIST)
				fprintf(stdout,"Nota: canalul fifo exista deja !\n");
			else
			{
				perror("Eroare la crearea canalului fifo. Cauza erorii");  exit(0);
			}
		}
        }

        if( mkfifo("canal-extern-out", 0666) == -1 )
        {
                perror("Eroare la crearea fifo 'canal-extern-out'");
                exit(1);
        }
}

void master_work()
{
        int nr, flag;
        int i, sump, total = 0;
	int fd_out, fd_in;
	char nume[100], buffer[10];

        printf("Introduceti numerele (0 pentru terminare):\n");
        flag = 1;

	for(i = 0; i < nrwork; i++)
	{
		strcpy(nume, "canal-extern-");
                snprintf(buffer, sizeof(buffer), "%d", i);
                strcat(nume, buffer);
		fifoi[i][1] = open(nume, O_WRONLY);
	}

        while(1)
        {
                scanf("%d", &nr);
                if(nr == 0) break;

                flag = (flag + 1) % nrwork;

                write( fifoi[flag][1], &nr, sizeof(int));

        }

	for(i = 0; i < nrwork; i++)
		close(fifoi[i][1]);

	fd_in = open("canal-extern-out", O_RDONLY);

        while( read(fd_in , &sump, sizeof(int) ) )
                total += sump;

        close(fd_in);

        printf("Master[PID:%d]> Suma secventei de numere introduse este: %d\n", getpid(), total);
}

void slave_work(int fdi, int fdo, int who)
{
        int nr, cod_r, suma_partiala;
        suma_partiala=0;
        do
        {
                cod_r = read(fdi, &nr, sizeof(int));
                switch(cod_r)
                {
                        case sizeof(int) : suma_partiala += nr ;  break;
                        case 0  : break;
                        default : fprintf(stderr,"Eroare la citirea din canalul pipei %d\n", who);
                                  exit(3);
                }
        }while(cod_r != 0);

        close(fdi);

        if( write(fdo, &suma_partiala, sizeof(int)) == -1)
        {
                perror("Eroare la scrierea in canalul pipeo");
                exit(4);
        }

        close(fdo);

        printf("Slave %d[PID:%d]> Suma partiala: %d\n", who, getpid(), suma_partiala);
}
