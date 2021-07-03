#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

void bubble_sort_cu_blocaje(int filedescr);

int main(int argc,char *argv[])
{
	int fd;
	if(argc < 2)
	{
		printf("Usage: %s datafile\n", argv[0]);  exit(1);
	}

	if(-1 == (fd = open(argv[1],O_RDWR)))
	{
		perror("Eroare la deschiderea fisierului de date");  exit(2);
	}
    
	bubble_sort_cu_blocaje(fd);

	close(fd);

	return 0;
}


void bubble_sort_cu_blocaje(int filedescr)
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

	/* bucla in care facem parcurgeri repetate */
	while(modificare)
	{
		modificare = 0; /* va fi setat daca se face macar o inversiune la parcurgerea curenta */

		/* bucla pentru o singura parcurgere a fisierului */
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

			/* daca este inversiune, le schimbam in fisier
			   Observatie: procedam similar ca la versiunea demo access4.c de la curs: intai punem blocajul, apoi verificam
			               daca niciunul dintre cele doua numere nu a fost cumva schimbat intre timp de o alta instanta de executie
			               a programului, si doar in caz afirmativ facem rescrierea!
			      Remarcă: de fapt, conditia de mai sus poate fi relaxata: chiar daca intre timp a fost schimbat vreunul dintre cele doua numere,
			               tot putem inversa cele doua numere, mai exact valorile noi ale acestora, daca este cazul sa facem inversiune, i.e.
			               daca nu sunt deja ordonate crescator.
			*/ 
			if(numar1 > numar2)
			{			
				/* ne intoarcem inapoi cu 2 intregi pentru a face verificarea si apoi, eventual, rescrierea */
				if(-1 == lseek(filedescr, -2*sizeof(int), SEEK_CUR))
				{
					perror("Eroare (1) la repozitionarea inapoi in fisier");  exit(5);
				}

				/* punem blocajul pe portiunea ce continea cele doua numere abia citite */
				if(-1 == fcntl(filedescr, F_SETLKW, &lacat_blocaj))
				{
					perror("Eroare la blocaj");  exit(10);
				}
	
				/* recitim cele 2 numere */
				if(-1 == read(filedescr, &numar1_recitit, sizeof(int)))
				{
					perror("Eroare la recitirea primului numar dintr-o pereche");  exit(11);
				}	

				if(-1 == read(filedescr, &numar2_recitit, sizeof(int)))
				{
					perror("Eroare la recitirea celui de-al doilea numar dintr-o pereche");  exit(12);
				}	
			
				/* daca numerele au ramas neschimbate, atunci efectuam rescrierea/inversiunea:
					    if( (numar1 == numar1_recitit) && (numar2 == numar2_recitit) ) ... */
				/* Sau si mai bine: chiar daca numerele nu au ramas neschimbate, daca este cazul efectuam inversiunea valorilor recitite dupa punerea blocajului:  */
				if(numar1_recitit > numar2_recitit)
				{
					modificare = 1;

					/* ne intoarcem inapoi cu 2 intregi pentru a face rescrierea */
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
			
				/* indiferent daca am facut sau nu inversiune, acum trebuie deblocata portiunea blocata mai sus
				   (i.e. scoatem blocajul de pe portiunea ce continea cele doua numere)                        */
				if(-1 == fcntl(filedescr, F_SETLKW, &lacat_deblocaj))
				{
					perror("Eroare la deblocaj");  exit(16);
				}
			}
		
			/* pregatim urmatoarea iteratie: primul numar din noua pereche este ce-al doilea numar din perechea precedenta */
			if(-1 == lseek(filedescr, -sizeof(int), SEEK_CUR))
			{
				perror("Eroare (2) la repozitionarea inapoi in fisier");  exit(8);
			}
		}/* sfarsitul buclei pentru o singura parcurgere a fisierului */

		/* pregatim urmatoarea parcurgere: ne repozitionam la inceputul fisierului */
		if(-1 == lseek(filedescr, 0L, SEEK_SET))
		{
			perror("Eroare (3) la repozitionarea inapoi in fisier");  exit(9);
		}

    }/* sfarsitul buclei de parcurgeri repetate */

}

