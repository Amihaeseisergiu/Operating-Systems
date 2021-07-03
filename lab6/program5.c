#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <pwd.h>

char* printPerm(char *file){
    	struct stat st;
	char* ret=malloc(sizeof(char)*9+1);
    	if(stat(file, &st) == 0)
	{
        	ret[0] = (st.st_mode & S_IRUSR) ? 'r' : '-';
        	ret[1] = (st.st_mode & S_IWUSR) ? 'w' : '-';
        	ret[2] = (st.st_mode & S_IXUSR) ? 'x' : '-';
        	ret[3] = (st.st_mode & S_IRGRP) ? 'r' : '-';
                ret[4] = (st.st_mode & S_IWGRP) ? 'w' : '-';
                ret[5] = (st.st_mode & S_IXGRP) ? 'x' : '-';
		ret[6] = (st.st_mode & S_IROTH) ? 'r' : '-';
                ret[7] = (st.st_mode & S_IWOTH) ? 'w' : '-';
                ret[8] = (st.st_mode & S_IXOTH) ? 'x' : '-';
		ret[9] = '\0';
		return ret;
    	}
    	else  exit(3);
}

void set(char *file,char opt)
{
	struct stat st;
	if(stat(file,&st)==0)
        {
                switch(opt)
                {
                       case '1': chmod(file,((st.st_mode & S_IRUSR) ? !S_IRUSR : S_IRUSR)|((st.st_mode & S_IWUSR) ? S_IWUSR : !S_IWUSR)|((st.st_mode & S_IXUSR) ? S_IXUSR : !S_IXUSR)|((st.st_mode & S_IRGRP) ? S_IRGRP : !S_IRGRP)|((st.st_mode & S_IWGRP) ? S_IWGRP : !S_IWGRP)|((st.st_mode & S_IXGRP) ? S_IXGRP : !S_IXGRP)|((st.st_mode & S_IROTH) ? S_IROTH : !S_IROTH)|((st.st_mode & S_IWOTH) ? S_IWOTH : !S_IWOTH)|((st.st_mode & S_IXOTH) ? S_IXOTH : !S_IXOTH)); break;
                       case '2': chmod(file,((st.st_mode & S_IRUSR) ? S_IRUSR : !S_IRUSR)|((st.st_mode & S_IWUSR) ? !S_IWUSR : S_IWUSR)|((st.st_mode & S_IXUSR) ? S_IXUSR : !S_IXUSR)|((st.st_mode & S_IRGRP) ? S_IRGRP : !S_IRGRP)|((st.st_mode & S_IWGRP) ? S_IWGRP : !S_IWGRP)|((st.st_mode & S_IXGRP) ? S_IXGRP : !S_IXGRP)|((st.st_mode & S_IROTH) ? S_IROTH : !S_IROTH)|((st.st_mode & S_IWOTH) ? S_IWOTH : !S_IWOTH)|((st.st_mode & S_IXOTH) ? S_IXOTH : !S_IXOTH)); break;
                       case '3': chmod(file,((st.st_mode & S_IRUSR) ? S_IRUSR : !S_IRUSR)|((st.st_mode & S_IWUSR) ? S_IWUSR : !S_IWUSR)|((st.st_mode & S_IXUSR) ? !S_IXUSR : S_IXUSR)|((st.st_mode & S_IRGRP) ? S_IRGRP : !S_IRGRP)|((st.st_mode & S_IWGRP) ? S_IWGRP : !S_IWGRP)|((st.st_mode & S_IXGRP) ? S_IXGRP : !S_IXGRP)|((st.st_mode & S_IROTH) ? S_IROTH : !S_IROTH)|((st.st_mode & S_IWOTH) ? S_IWOTH : !S_IWOTH)|((st.st_mode & S_IXOTH) ? S_IXOTH : !S_IXOTH)); break;
                       case '4': chmod(file,((st.st_mode & S_IRUSR) ? S_IRUSR : !S_IRUSR)|((st.st_mode & S_IWUSR) ? S_IWUSR : !S_IWUSR)|((st.st_mode & S_IXUSR) ? S_IXUSR : !S_IXUSR)|((st.st_mode & S_IRGRP) ? !S_IRGRP : S_IRGRP)|((st.st_mode & S_IWGRP) ? S_IWGRP : !S_IWGRP)|((st.st_mode & S_IXGRP) ? S_IXGRP : !S_IXGRP)|((st.st_mode & S_IROTH) ? S_IROTH : !S_IROTH)|((st.st_mode & S_IWOTH) ? S_IWOTH : !S_IWOTH)|((st.st_mode & S_IXOTH) ? S_IXOTH : !S_IXOTH)); break;
                       case '5': chmod(file,((st.st_mode & S_IRUSR) ? S_IRUSR : !S_IRUSR)|((st.st_mode & S_IWUSR) ? S_IWUSR : !S_IWUSR)|((st.st_mode & S_IXUSR) ? S_IXUSR : !S_IXUSR)|((st.st_mode & S_IRGRP) ? S_IRGRP : !S_IRGRP)|((st.st_mode & S_IWGRP) ? !S_IWGRP : S_IWGRP)|((st.st_mode & S_IXGRP) ? S_IXGRP : !S_IXGRP)|((st.st_mode & S_IROTH) ? S_IROTH : !S_IROTH)|((st.st_mode & S_IWOTH) ? S_IWOTH : !S_IWOTH)|((st.st_mode & S_IXOTH) ? S_IXOTH : !S_IXOTH)); break;
                       case '6': chmod(file,((st.st_mode & S_IRUSR) ? S_IRUSR : !S_IRUSR)|((st.st_mode & S_IWUSR) ? S_IWUSR : !S_IWUSR)|((st.st_mode & S_IXUSR) ? S_IXUSR : !S_IXUSR)|((st.st_mode & S_IRGRP) ? S_IRGRP : !S_IRGRP)|((st.st_mode & S_IWGRP) ? S_IWGRP : !S_IWGRP)|((st.st_mode & S_IXGRP) ? !S_IXGRP : S_IXGRP)|((st.st_mode & S_IROTH) ? S_IROTH : !S_IROTH)|((st.st_mode & S_IWOTH) ? S_IWOTH : !S_IWOTH)|((st.st_mode & S_IXOTH) ? S_IXOTH : !S_IXOTH)); break;
                       case '7': chmod(file,((st.st_mode & S_IRUSR) ? S_IRUSR : !S_IRUSR)|((st.st_mode & S_IWUSR) ? S_IWUSR : !S_IWUSR)|((st.st_mode & S_IXUSR) ? S_IXUSR : !S_IXUSR)|((st.st_mode & S_IRGRP) ? S_IRGRP : !S_IRGRP)|((st.st_mode & S_IWGRP) ? S_IWGRP : !S_IWGRP)|((st.st_mode & S_IXGRP) ? S_IXGRP : !S_IXGRP)|((st.st_mode & S_IROTH) ? !S_IROTH : S_IROTH)|((st.st_mode & S_IWOTH) ? S_IWOTH : !S_IWOTH)|((st.st_mode & S_IXOTH) ? S_IXOTH : !S_IXOTH)); break;
                       case '8': chmod(file,((st.st_mode & S_IRUSR) ? S_IRUSR : !S_IRUSR)|((st.st_mode & S_IWUSR) ? S_IWUSR : !S_IWUSR)|((st.st_mode & S_IXUSR) ? S_IXUSR : !S_IXUSR)|((st.st_mode & S_IRGRP) ? S_IRGRP : !S_IRGRP)|((st.st_mode & S_IWGRP) ? S_IWGRP : !S_IWGRP)|((st.st_mode & S_IXGRP) ? S_IXGRP : !S_IXGRP)|((st.st_mode & S_IROTH) ? S_IROTH : !S_IROTH)|((st.st_mode & S_IWOTH) ? !S_IWOTH : S_IWOTH)|((st.st_mode & S_IXOTH) ? S_IXOTH : !S_IXOTH)); break;
                       case '9': chmod(file,((st.st_mode & S_IRUSR) ? S_IRUSR : !S_IRUSR)|((st.st_mode & S_IWUSR) ? S_IWUSR : !S_IWUSR)|((st.st_mode & S_IXUSR) ? S_IXUSR : !S_IXUSR)|((st.st_mode & S_IRGRP) ? S_IRGRP : !S_IRGRP)|((st.st_mode & S_IWGRP) ? S_IWGRP : !S_IWGRP)|((st.st_mode & S_IXGRP) ? S_IXGRP : !S_IXGRP)|((st.st_mode & S_IROTH) ? S_IROTH : !S_IROTH)|((st.st_mode & S_IWOTH) ? S_IWOTH : !S_IWOTH)|((st.st_mode & S_IXOTH) ? !S_IXOTH : S_IXOTH)); break;
                }
        }
        else
        {
                printf("Eroare stat pentru %s\n",file);
                exit(4);
	}
}

int main(int argc,char *argv[])
{
	if(argc <2)
	{
		printf("Eroare:nu a fost specificat un fisier!\n");
		exit(1);
	}
	if(access(argv[1],R_OK)==-1)
	{
		printf("Eroare: fisierul nu exista!\n");
		exit(2);
	}
	char c[10];
	while(1)
	{
		printf("Permisiunile pentru %s:\n###########\n#UUUGGGOOO#\n#|||||||||#\n#%s#\n#|||||||||#\n#123456789#\n###########\nModifica (1-9) | Iesire 0: ",argv[1],printPerm(argv[1]));
		scanf("%s",c);
		if(c[0]=='0' || strcmp(c,"exit")==0)break;
		int i;
		for(i=0;i<strlen(c);i++)
			set(argv[1],c[i]);
		printf("\033[A\r\33[2K\033[A\r\33[2K\033[A\r\33[2K\033[A\r\33[2K\033[A\r\33[2K\033[A\r\33[2K\033[A\r\33[2K\033[A\r\33[2K\033[A\r\33[2K");
	}
	printf("\033[A\r\33[2K\033[A\r\33[2K\033[A\r\33[2K\033[A\r\33[2K\033[A\r\33[2K\033[A\r\33[2K\033[A\r\33[2K\033[A\r\33[2K\033[A\r\33[2K");
	return 0;
}
