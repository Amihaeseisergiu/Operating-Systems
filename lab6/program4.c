#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <pwd.h>
#include <libgen.h>

void parcurgere_director(char *cale);

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        fprintf(stderr,"Eroare: numar insuficient de parametri!\n");
        exit(1);
    }

    parcurgere_director(argv[1]);

    return 0;
}

int isDirectory(const char *path) {
   struct stat statbuf;
   if (stat(path, &statbuf) != 0)
       return 0;
   return S_ISDIR(statbuf.st_mode);
}

int isLink (const char *path) {
    struct stat statbuf;
    if (lstat(path, &statbuf) != 0)
	return 0;
    return S_ISLNK(statbuf.st_mode);
}

void parcurgere_director(char *cale)
{
    DIR *dir;
    struct dirent *de;
    char nume[PATH_MAX];
    if(isDirectory(cale) != 1)
    {
        if(isLink(cale) == 1 && access(cale,R_OK) == -1)unlink(cale);
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
                parcurgere_director(nume);

	    }
	}
	closedir(dir);
    }
}
