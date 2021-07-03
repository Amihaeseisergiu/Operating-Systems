#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>

int main (int argc, char* argv[])
{
        char ch,ch1=argv[3][0],ch2=argv[4][0];
        int fin,fout;
        if(argc < 5)
        {
                fprintf(stderr,"Argumente insuficiente!");
                exit(1);
        }
        if( -1 != access(argv[2],R_OK) )
        {
                fprintf(stderr,"Fisierul %s exista.Suprascrieti? (y/n): ", argv[2]);
                int ans;
                ans=getchar();
                if(ans == 'n')exit(2);
                fout = open(argv[2], O_WRONLY);
        }
        else
        {
                fout = creat(argv[2],O_WRONLY);
                chmod(argv[2],S_IRUSR|S_IWUSR);
        }
        fin = open(argv[1], O_RDONLY);
        while(1)
        {
                int r = read(fin,&ch,1);
                if(r == -1)
                {
                        perror("Eroare la citire");
                        exit(3);
                }
                if(r == 0)break;
                if(ch == ch1)write(fout,&ch2,1);
                else write(fout,&ch,1);
        }
        close(fin);
        close(fout);
        return 0;
}

