#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int background;

void tokenize(char *line, char **argv)
{
	char* p;
	p = strtok(line," \t\n");
	while(p)
	{
		*argv++ = p;
		p = strtok(NULL," \t\n");
	}
	*argv = NULL;
}

void execute(char **argv)
{
     	pid_t  pid;
     	int status;

     	if ((pid = fork()) == -1)
	{
       		  printf("Eroare la fork\n");
        	  exit(1);
     	}
     	else if (pid == 0)
	{
         	 if (execvp(*argv, argv) < 0)
		 {
               	 	printf("Eroare la exec\n");
               	 	exit(1);
        	}
     	}
     	else
	{
          	if(background == 0)while (wait(&status) != pid);
     	}
}

int main()
{
     	char  line[1000];
     	char  *argv[100] = {"clear",NULL};

	execute(argv);

     	while(1)
	{
        	printf("MyShell> ");
        	if(!fgets(line, 1000, stdin))break;
		size_t length = strlen(line);
		if(line[length - 1] == '\n')line[length - 1] = '\0';

		size_t lenght = strlen(line);
		if(line[lenght - 1] == '&')
		{
			background = 1;
			line[lenght - 1] = '\0';
		}
			else background = 0;

        	tokenize(line, argv);
        	if( (strcmp(argv[0], "exit") == 0) || (strcmp(argv[0], "logout") == 0) )exit(0);

          	execute(argv);
     	}

    	return 0;
}
