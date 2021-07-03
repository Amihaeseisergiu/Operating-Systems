#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int which = 0, done = 0;

int find(char *line)
{
	int i = 0, ct = 0;
	while(1)
	{
		if(line[i] == '\0')return i;
		if(ct == which)
		{
			which++;
			if(ct == 0)return 0;
				else return i+1;
		}
		if(line[i] == '|')ct++;
		i++;
	}
}

void tokenize(char* line, char** arg)
{
	char* p;
        p = strtok(line," \t\n");
        while(p)
        {
		if(strcmp(p,"|") == 0)
		{
			done = 0;
			break;
		}
		done = 1;
                *arg++ = p;
                p = strtok(NULL," \t\n");
        }
        *arg = NULL;
}

int create_sub_process(char *const command[], int input_stream)
{
    	int pipefd[2] = {-1, -1};
    	pid_t fk;

    	if (pipe(pipefd) < 0)
    	{
        	perror("pipe");
        	close(input_stream);
        	return -1;
    	}

    	if ((fk = fork()) < 0)
    	{
        	perror("fork");
        	close(pipefd[0]);
        	close(pipefd[1]);
        	close(input_stream);
        	return -1;
    	}

    	if (fk == 0)
    	{
        	close(pipefd[0]);

        	close(0);
        	dup(input_stream);
        	close(input_stream);

        	close(1);
        	dup(pipefd[1]);
        	close(pipefd[1]);

        	execvp(command[0], command);
        	perror("execvp");
        	exit(1);
    	}

    	close(input_stream);
    	close(pipefd[1]);

    	return pipefd[0];
}

int main()
{
	char line[1000], copy[1000];
    	int fd;
    	int i;
	while(1)
        {
                printf("UNIX> ");
                if(!fgets(line, 1000, stdin))break;
                size_t length = strlen(line);
                if(line[length - 1] == '\n')line[length - 1] = '\0';

		fd = dup(0);
		while(done == 0)
		{
			char *arg[100];
			i = find(line);
			strcpy(copy,line+i);
			tokenize(copy,arg);
                	fd = create_sub_process(arg, fd);
			if (fd < 0)
        	        {
                                exit(1);
	                }

		}

		close(0);
    		dup(fd);
    		close(fd);
		execlp("cat", "cat", (char *)NULL);
    		perror("execlp");

        }

    return 0;
}
