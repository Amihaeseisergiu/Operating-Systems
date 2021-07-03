#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

void calcul_tata(int N)
{
	FILE *fs, *fd;
	char str[2];
	int nr1, nr2, i;

	if ( NULL == ( fs = fopen("operanzi.bin","r") ) )
	{
		perror("Eroare la deschiderea fisierului operanzi.bin");
		exit(0);
	}

	if ( NULL == ( fd = fopen("operatori.txt","r") ) )
        {
                perror("Eroare la deschiderea fisierului operatori.txt");
                exit(1);
        }

	for(i = 1; i <= N; i++)
	{
		fscanf(fs,"%d",&nr1);
		fscanf(fs,"%d",&nr2);
		fscanf(fd,"%s",str);
		if(str[0] == '+')printf("Calculul %d+%d=%d\n",nr1,nr2,(nr1+nr2));
		else if(str[0] == '-')printf("Calculul %d-%d=%d\n",nr1,nr2,(nr1-nr2));
		else if(str[0] == '*')printf("Calculul %d*%d=%d\n",nr1,nr2,(nr1*nr2));
		else
		{
			float nf1 = nr1;
			float nf2 = nr2;
			printf("Calculul %d/%d=%f\n",nr1,nr2,(nf1/nf2));
		}
	}
	fclose(fs);
	fclose(fd);
}

void generare_fiu1(int N)
{
	FILE *fd;
	int nr1, nr2, i;

	srandom(getpid());

	if( NULL == ( fd = fopen("operanzi.bin","w+") ) )
	{
		perror("Eroare la deschiderea fisierului operanzi.bin");
		exit(0);
	}

	for(i = 1; i <= N; i++)
	{
		nr1 = random() % 100;
		nr2 = random() % 100;
		fprintf(fd,"%d %d ",nr1,nr2);
	}
	fclose(fd);
}

void generare_fiu2(int N)
{
	FILE *fd;
        int nr1, i;

        srandom(getpid());

        if( NULL == ( fd = fopen("operatori.txt","w+") ) )
        {
                perror("Eroare la deschiderea fisierului operatori.txt");
                exit(0);
        }

        for(i = 1; i <= N; i++)
        {
                nr1 = random() % 4;
                if(nr1 == 0)fprintf(fd,"+ ");
		else if(nr1 == 1)fprintf(fd,"- ");
		else if(nr1 == 2)fprintf(fd,"* ");
		else fprintf(fd,"/ ");
        }
        fclose(fd);

}


int main()
{
	pid_t pid_fiu1, pid_fiu2, N;

	printf("Dati numarul intreg N:"); scanf("%d", &N); // TODO: tratați excepțiile de citire a unui număr întreg

	if( N < 0 )
	{
		printf("Eroare: numar negativ!\n");
		exit(0);
	}

	/* crearea procesului fiu #1 */
	if(-1 == (pid_fiu1=fork()) )
	{
		perror("Eroare la fork #1");  return 1;
	}

	/* ramificarea executiei dupa primul apel fork */
	if(pid_fiu1 == 0)
	{ /* zona de cod executata doar de catre fiul #1 */
		printf("\n[P1] Procesul fiu 1, cu PID-ul: %d.\n", getpid());

		generare_fiu1(N);
		return 0;
	}
	else
	{ /* zona de cod executata doar de catre parinte */
		printf("\n[P0] Procesul tata, cu PID-ul: %d.\n", getpid());

		/* crearea procesului fiu #2 */
		if(-1 == (pid_fiu2=fork()) )
		{
			perror("Eroare la fork #2");  return 2;
		}

		/* ramificarea executiei dupa al doilea apel fork */
		if(pid_fiu2 == 0)
		{ /* zona de cod executata doar de catre fiu */
			printf("\n[P2] Procesul fiu 2, cu PID-ul: %d.\n", getpid());

			generare_fiu2(N);
			return 0;
		}
		else
		{
			wait(NULL); wait(NULL); // mai intai astept terminarea fiilor, si apoi fac calculul

			calcul_tata(N);
			return 0;
		}
	}
}
