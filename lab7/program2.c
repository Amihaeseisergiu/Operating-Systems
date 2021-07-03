#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char* argv[])
{
	char *nume_sursa = NULL;
	FILE *fs;
	int cnt, i = 0, first = 0, iterator = 1, noption = 0, nargs = 0, coption = 0, cargs = 0, countn = 0, countc = 0, plusc = 0, plusn = 0, plus = 0;
	if( argc < 1 )
	{
		 fprintf(stderr,"Ati uitat sa specificati numele fisierului\n");
	 	 exit(1);
	}
	while(iterator < argc)
	{
		if( strcmp(argv[iterator],"-n") == 0)
		{
			noption = 1;
 			nargs = atoi(argv[iterator+1]);
			if(argv[iterator+1][0] == '+')plusn = 1;
			iterator++;
			first = 1;
		}
		else if( strcmp(argv[iterator],"-c") == 0)
		{
			coption = 1;
			cargs = atoi(argv[iterator+1]);
			if(argv[iterator+1][0] == '+')plusc = 1;
			iterator++;
			first = 2;
		}
		else if(!nume_sursa)nume_sursa = argv[iterator];
		iterator++;
	}
	if(noption + coption == 2)
	{
		if(first == 1)coption = 0;
		else if(first == 2)noption = 0;
	}
	else if(noption + coption == 0)
	{
		noption = 1;
		nargs = 10;
	}
	if( (fs = fopen(nume_sursa,"r")) == NULL )
	{
		perror("Eroare la deschiderea fisierului de prelucrat");
		exit(2);
	}
    	fseek(fs,0,SEEK_END);
    	cnt = ftell(fs);
	int in = 0;
	if((coption == 1 && plusc == 0) || (noption == 1 && plusn == 0))
    	while( i < cnt )
    	{
        	i++;
        	fseek(fs,-i,SEEK_END);
		if((countn > nargs) || (countc > cargs))
		{
			in = 1;
			int curr = i-2;
			while( curr > 0 )
			{
				fseek(fs,-curr,SEEK_END);
				printf("%c",fgetc(fs));
				curr--;
			}
			break;
		}
		if(noption == 1 && fgetc(fs) == '\n' && countn <= nargs)countn++;
		if(coption == 1 && countc <= cargs)countc++;
    	}
	else
	{
			int count = 0;
			int ch;
			while((count < nargs || count < cargs) && i < cnt)
			{
				fseek(fs,i,SEEK_SET);
				i++;
				ch=fgetc(fs);
				if(ch == '\n' && noption == 1)count++;
				else if(coption == 1)count++;
			}
			while(i < cnt)
			{
				fseek(fs,i,SEEK_SET);
				i++;
				ch=fgetc(fs);
				printf("%c",ch);
			}
	}
	if(in == 0 && (plusc == 0 && plusn == 0))
	{
		while( cnt > 0 )
		{
			fseek(fs,-cnt,SEEK_END);
			printf("%c",fgetc(fs));
			cnt--;
		}
	}
        fclose(fs);
	return 0;
}
