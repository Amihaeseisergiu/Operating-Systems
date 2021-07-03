#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>

int main(int argc, char* argv[])
{
	int p[2], q[2], r[2], ap[26] = {0}, i, fd1, out, total;
	char ch;
	FILE* fd2;
	pid_t pid1, pid2;
	pipe(p);
	pipe(q);
	pipe(r);
	if(-1 == (pid1 = fork() ) )
	{
		perror("Eroare la fork1():");
		exit(0);
	}
	else
	{
		if(pid1 == 0)
		{
			close(p[1]);
			while(1)
			{
				if(read(p[0],&ch,1) <= 0)break;
				fflush(stdout);
				if(ch >= 'a' && ch <= 'z')
					write(q[1],&ch,1);
			}
			close(p[0]);
		}
		else
		{
			if(-1 == (pid2 = fork() ) )
			{
				perror("Eroare la fork2():");
				exit(1);
			}
			else
			{
				if(pid2 == 0)
				{
					close(q[1]);
					close(p[1]);
					if(NULL == (fd2 = fopen("statistica.txt","w+") ) )
					{
						perror("Eroare la deschiderea fisierului statistica.txt:");
					}
					while(1)
					{
						if(read(q[0],&ch,1) <= 0 )break;
						ap[ch - 'a']++;
					}
					close(q[0]);
					total = 0;
					for(i = 0; i < 26; i++)
					{
						if(ap[i] != 0)
						{
							fprintf(fd2,"%c %d\n",i+'a',ap[i]);
							total++;
						}
					}
					write(r[1],&total,4);
					close(r[1]);
				}
				else
				{
					if( -1 == ( fd1 = open("date.txt",O_RDONLY) ) )
					{
						perror("Eroare la deschiderea fisierului date.txt:");
						exit(2);
					}
					while(1)
					{
						if(read(fd1,&ch,1) <= 0)break;
						write(p[1],&ch,1);
					}
					close(p[1]);
					close(q[1]);
					read(r[0],&out,4);
					close(r[0]);
					printf("Numarul: %d\n",out);
				}
			}
		}
	}
	return 0;
}
