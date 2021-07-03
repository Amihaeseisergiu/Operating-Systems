#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
	int p[2], q[2], fn, fp, fo, rcod, rcod2, ct1, ct2;
	char ch, ch2, out, out2, wr;
	pid_t pid1, pid2;
	pipe(p);
	pipe(q);
	wr = '1';
	write(p[1], &wr, 1);
	if(-1 == (fn = open("nume.txt", O_RDONLY) ) )
	{
		perror("Eroare la deschiderea fisierului nume.txt:");
		exit(0);
	}
	if(-1 == (fp = open("prenume.txt", O_RDONLY) ) )
	{
		perror("Eroare la deschiderea fisierului prenume.txt:");
		exit(1);
	}
	if(-1 == (fo = open("persoane.txt", O_RDWR | O_CREAT | O_TRUNC, 0600) ) )
	{
		perror("Eroare la deschiderea fisierului persoane.txt:");
		exit(5);
	}
	if(-1 == (pid1 = fork() ) )
	{
		perror("Eroare la fork1():");
	}
	else
	{
		if(pid1 == 0)
		{
			close(q[1]);
			close(p[0]);
			while(1)
			{
				read(q[0], &out, 1);
				printf(" ");
				ch = ' ';
				write(fo, &ch, 1);
				ct1 = 0;
				while(1)
				{
					rcod = read(fp, &ch, 1);
					if(ch == '\n' || rcod == 0)break;
					printf("%c",ch);
					write(fo,&ch,1);
				}
				fflush(stdout);
				printf("\n");
				ch = '\n';
				write(fo,&ch,1);
				wr = '1';
				write(p[1], &wr, 1);
				if(rcod == 0 || out == '3')break;
			}
			wr = '2';
			write(p[1], &wr, 1);
			close(p[1]);
			close(q[0]);
			fflush(stdout);
		}
		else
		{
			if(-1 == (pid2 = fork() ) )
			{
				perror("Eroare la fork2():");
			}
			else
			{
				if(pid2 == 0)
				{
					close(p[1]);
					close(q[0]);
					while(1)
                        		{
                               	 		read(p[0], &out2, 1);
						ct2 = 0;
                                		while(1)
                                		{
                                        		rcod2 = read(fn, &ch2, 1);
                                        		if(ch2 == '\n' || rcod2 == 0)break;
                                        		printf("%c",ch2);
							write(fo,&ch2,1);
                                		}
						fflush(stdout);
						wr = '1';
                                		write(q[1], &wr, 1);
                                		if(rcod2 == 0 || out2 == '2')break;
                        		}
					wr = '3';
					write(q[1], &wr, 1);
					close(p[0]);
					close(q[1]);
					fflush(stdout);
				}
				else
				{
					sleep(1);
					close(p[0]);
					close(p[1]);
					close(q[0]);
					close(q[1]);
					wait(NULL);
					wait(NULL);
				}
			}
		}
	}
	return 0;
}
