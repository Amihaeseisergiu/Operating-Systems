#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char* argv[])
{
	char nume_dir[100],op;
	int cod_term;

	if(argc > 1)
	{
		strcpy(nume_dir,argv[1]);
	}
	else
	{
		printf("Introduceti numele directorului dorit:");  scanf("%s", nume_dir);
	}

	switch( fork() )
	{
		case -1:	perror("Eroare la primul fork");  exit(1);

		case  0:	execlp("ls","ls","-l",nume_dir,NULL);
				perror("Eroare la primul exec");  exit(2);

		default:	wait(&cod_term);
				if( WIFEXITED(cod_term) )
					printf("Comanda ls a rulat, terminandu-se cu codul de terminare: %d.\n", WEXITSTATUS(cod_term) );
				else
					printf("Comanda ls a fost terminata fortat de catre semnalul: %d.\n", WTERMSIG(cod_term) );
	}

	printf("Stergeti directorul? (y/n) \n");
	scanf("%c",&op);
	if(op == 'n')return 1;
	else
	{
		switch( fork() )
        	{
                	case -1:        perror("Eroare la al doilea fork");  exit(1);

                	case  0:        execv("/bin/rm",(char *[]){"/bin/rm","-r","-i",nume_dir,NULL});
					perror("Eroare la al doilea exec");  exit(2);

                	default:        wait(&cod_term);
                        	        if( WIFEXITED(cod_term) )
                                	        printf("Comanda rm a rulat, terminandu-se cu codul de terminare: %d.\n", WEXITSTATUS(cod_term) );
                                	else
                                        	printf("Comanda rm a fost terminata fortat de catre semnalul: %d.\n", WTERMSIG(cod_term) );
        	}
	}

	return 0;
}
