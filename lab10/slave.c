#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char* argv[])
{
	int rezultat, nr1, nr2;

	nr1 = atoi( argv[1] );
	nr2 = atoi( argv[2] );

	if(argv[3][0] == '-')
	{
		rezultat = nr1 - nr2;
		exit(rezultat);
	}
	else
	{
		rezultat = nr1 + nr2;
		exit(rezultat);
	}
	return 0;
}

