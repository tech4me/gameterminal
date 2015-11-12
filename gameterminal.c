#include <string.h>
#include <inttypes.h>
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int pfork(int x);
bool checkFile();
void runPrograms(char colourA,char colourB, char n);

int main(int argc, char* argv[])//"boardsize" "BW or WB" BW= AB BW | WB= AW BB
{
    char colourA, colourB;
    colourA = *argv[2];
    colourB = *( argv[2] + 1 );

    if ( checkFile())
    {
        runPrograms(colourA,colourB,*(argv[1]));
    }
    else
        printf("Not able to find executables 'A' and 'B',exiting!\n");
    return (EXIT_SUCCESS);
}

bool checkFile(void)
{
    int counter = 0;
    DIR *d;
    struct dirent *dir;
    d = opendir(".");
    if ( d )
    {
        while (( dir = readdir(d)) != NULL )
        {
            if ( dir->d_type == DT_REG )
            {
                if (( *( dir->d_name ) == 'A' || *( dir->d_name ) == 'B' ) && ( *( dir->d_name + 1 ) != '.' ))
                    counter++;
                //printf("%s\n", dir->d_name);
            }
        }
        closedir(d);
    }
    if ( counter == 2 )
        return true;
    else
        return false;
}

void runPrograms(char colourA, char colourB,char n)
{
    char a;
    pid_t pid;
    int rv;
    int parentA[2];    // The four pipes
    int childA[2];

    int parentB[2];
    int childB[2];

    char buffA[1000] ={0};
    char buffB[1000] ={0};


    if ( pipe(parentA) || pipe(childA) || pipe(parentB) || pipe(childB) )
    {
        fprintf(stderr, "Pipe error!\n");
        exit(1);
    }

    int i;
    i = pfork(3);

    if ( i == 0 )    // A positive (non-negative) PID indicates the parent process    // parent
    {
        setvbuf(stdout, (char*) NULL, _IONBF, 0);
        int inA, outA;
        inA = parentA[0];
        outA = childA[1];
        close(parentA[1]);
        close(childB[0]);

        int inB, outB;
        inB = parentB[0];
        outB = childB[1];
        close(parentB[1]);
        close(childB[0]);

		
	char enter = '\n';
	memset(buffA,0,strlen(buffA));
	memset(buffB,0,strlen(buffB));

        read (inA, buffA, 1000);
        read (inB, buffB, 1000);
	printf("%s\n", buffA);
//	printf("%s\n", buffB);


	// write dimension
	write(outA, &n, 1);
	write(outA, &enter, 1);
	write(outB, &n, 1);
	write(outB, &enter, 1);

	usleep(50);	

	memset(buffA,0,strlen(buffA));
	memset(buffB,0,strlen(buffB));
        read (inA, buffA, 1000);
        read (inB, buffB, 1000);
	printf("%s\n", buffA);
//	printf("%s\n", buffB);


	// write colour
	write(outA, &colourA, 1);
	write(outA, &enter, 1);
	write(outB, &colourB, 1);
	write(outB, &enter, 1);

	usleep(500);

// LOOP
	read(inA, buffA, 1000);
	read(inB, buffB, 1000);
	int winner = 3;	// undecided
	while(winner == 3) {
	if (buffB[67] == ':') {
		char temp[3];
		temp[0] = buffA[56];
		temp[1] = buffA[57];
		temp[2] = '\n';		
		write(inB, temp, 3);
		usleep(500);
		memset(buffB,0,strlen(buffB));
  		read (inB, buffB, 1000);

	} 

	if (buffA[67] == ':') {
		char temp[3];
		temp[0] = buffB[56];
		temp[1] = buffB[57];
		temp[2] = '\n';		
		write(inA, temp, 3);
		usleep(500);
 		memset(buffA,0,strlen(buffA));
		read (inA, buffA, 1000);

	}
		printf("%s\n", buffA);
		printf("%s\n", buffB);

	}



    }
    else if ( i == 1 )            // child A
    // A zero PID indicates that this is the child process
    {
        setvbuf(stdout, (char*) NULL, _IONBF, 0);
        int in, out;
        in = childA[0];
        out = parentA[1];
        close(childA[1]);
        close(parentA[0]);

        dup2(in, 0);
        dup2(out, 1);

        if (execl("A", "A", NULL) == -1)
        fprintf(stderr, "A execl Error!\n");
        exit(1);
    }
    else if ( i == 2 )              // child B
    {
        setvbuf(stdout, (char*) NULL, _IONBF, 0);
        int in, out;
        in = childB[0];
        out = parentB[1];
        close(childB[1]);
        close(parentB[0]);
        dup2(in, 0);
        dup2(out, 1);

        if (execl("B", "B", NULL) == -1)
      		fprintf(stderr, "B execl Error!\n");
        exit(1);
    }


}

int pfork(int x)
{
    int j;
    for( j = 1; j < x; j++ )
    {
        if( fork() == 0 )
        {
            //sleep(3);
            return j;
        }
    }
    return 0;
}
