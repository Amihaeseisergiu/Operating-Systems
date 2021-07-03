#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char* argv[])
{
	char *nume_sursa = NULL, *nume_destinatie = NULL;
	FILE *fs, *fd;
	int cnt, i = 0, pos2;
	if( argc < 2 )
	{
		 fprintf(stderr,"Ati uitat sa specificati numele fisierelor\n");
	 	 exit(1);
	}
	nume_sursa = argv[1];
	nume_destinatie = argv[2];
	if( (fs = fopen(nume_sursa,"r")) == NULL || (fd = fopen(nume_destinatie,"w+")) == NULL )
	{
		perror("Eroare la deschiderea fisierelor de prelucrat");
		exit(2);
	}
    	fseek(fs,0,SEEK_END);
    	cnt = ftell(fs);
	pos2 = 0;
    	while( i < cnt )
    	{
        	i++;
        	fseek(fs,-i,SEEK_END);
		if(fgetc(fs) == '\n')
		{
			int curr = i;
			while( curr > pos2 )
			{
				fseek(fs,-curr,SEEK_END);
				fprintf(fd,"%c",fgetc(fs));
				curr--;
			}
			pos2 = i;
			fseek(fs,-i,SEEK_END);
		}
    	}
	fprintf(fd,"\n");
	int curr = i;
	while(curr > pos2)
	{
		fseek(fs,-curr,SEEK_END);
		fprintf(fd,"%c",fgetc(fs));
		curr--;
	}
	fprintf(fd,"\n");
        fclose(fs);
	fclose(fd);
	return 0;
}
