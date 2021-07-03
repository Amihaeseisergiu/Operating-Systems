#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

void tokenize(char *line, char** argv)
{
	char *p;
	p = strtok(line, " \t\n");
	while(p)
	{
		*argv++ = p;
		p = strtok(NULL, " \t\n");
	}
	*argv = NULL;
}

int main(void)
{
        int socket, rcod, cod, cont, pid;
        char line[100], ch;

        if(-1 == mkfifo("socket", 0600) )
        {
                if(errno != EEXIST)
                {
                        perror("Eroare la deschiderea canalului in supervisor:");
                        exit(1);
                }
        }

        while(1)
        {
                socket = open("socket", O_RDONLY);
		cont = 0;

                while(1)
                {
                        rcod = read(socket, &ch, 1);
                        if(rcod == 0 || ch == '\n')break;
                        line[cont++] = ch;
                }
		line[cont] = '\0';

                if(rcod == 0 || strcmp("exit",line) == 0)break;
                close(socket);

		char* argv[100];
		tokenize(line,argv);

		pid = fork();
		if(pid == -1)
		{
			perror("Eroare la fork():");
			exit(2);
		}

		if(pid == 0)
			execvp(argv[0],argv);
		wait(&cod);

                socket = open("socket", O_WRONLY);
                write(socket, &cod, 4);
                close(socket);
        }
	close(socket);
	cod = 99;
	socket = open("socket", O_WRONLY);
	write(socket,&cod,4);
	close(socket);
        return 0;
}

