
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <pwd.h>
#include <libgen.h>

void parcurgere_director(char *cale,char *find);

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
	fprintf(stderr,"Eroare nu a fost introdus un nume de fisier!\n");
	exit(2);
    }

    parcurgere_director("/fenrir/studs/sergiu.amihaesei",argv[1]);

    return 0;
}

int isDirectory(const char *path) {
   struct stat statbuf;
   if (stat(path, &statbuf) != 0)
       return 0;
   return S_ISDIR(statbuf.st_mode);
}

void parcurgere_director(char *cale,char *find)
{
    DIR *dir;
    struct dirent *de;
    char nume[PATH_MAX];
    if(isDirectory(cale) != 1)
    {
	char relativ[strlen(cale)];
	strcpy(relativ,cale+31);
	if(strcmp(basename(cale),find)==0)printf("%s\n",relativ);
        return;
    }
    else
    {
        if(NULL == (dir = opendir(cale)) )
        {
            fprintf(stderr,"Eroare deschidere director %s .\t",cale);  perror("Cauza este");
            return;
        }
        while(NULL != (de = readdir(dir)) )
        {
            if( strcmp(de->d_name,".") && strcmp(de->d_name,"..") )
            {
                sprintf(nume,"%s/%s",cale,de->d_name);
                parcurgere_director(nume,find);
            }
        }

        closedir(dir);
    }
}
