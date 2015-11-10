#include <string.h>
#include <inttypes.h>
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h>

int pfork(int x);
bool checkFile();
void runPrograms(char colourA,char colourB,int n);

int main(int argc, char* argv[])//"boardsize" "BW or WB" BW= AB BW | WB= AW BB
{
    int boardSize;
    char* endPtr;
    boardSize = strtoimax(argv[1], &endPtr, 10);
    char colourA, colourB;
    colourA = *argv[2];
    colourB = *( argv[2] + 1 );

    if ( checkFile())
    {
        runPrograms(colourA,colourB,boardSize);
    }
    else
        printf("Not able to find executables 'A' and 'B',exiting!\n");
    return ( EXIT_SUCCESS );
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

void runPrograms(char colourA,char colourB,int n)
{
    char a;
    pid_t pid;
    int rv;
    int parentA[2];    // The four pipes
    int childA[2];

    int parentB[2];
    int childB[2];

    char buffA[50];
    char buffB[50];


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

        write(outA, &colourA, 50);
        write(outB, &colourB, 50);
       // sleep(1);
      //  read (inA, buffA, 50);
        read (inB, buffB, 50);
        printf("%s\n", buffB);
        //write (outB, buff,strlen(buff) + 1);

        /*dup2(commpipeAParent[0], 0); // Replace stdin with the in side of the pipe
           close(commpipeAParent[1]); // Close unused side of pipe (out side) Replace the child fork with a new process
           {
           if (execl("A", "A", NULL) == -1)
           fprintf(stderr, "execl Error!");
           exit(1);
           }*/
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
        //write (out,hi,strlen(hi)+1);
        //read(in, buff, 50);
        //char temp[50];
        //scanf("%s", temp);
        //printf("%s", temp);

        if (execl("A", "A", NULL) == -1)
        fprintf(stderr, "A execl Error!\n");
        exit(1);
        /* dup2(commpipeAParent[1], 1); // Replace stdout with out side of the pipe
           close(commpipeAParent[0]); // Close unused side of pipe (in side)
           setvbuf(stdout, (char*) NULL, _IONBF, 0);  // Set non-buffered output on stdout
         */
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
        //read(in, buff, 50);
        // write(out, hi, strlen(hi)+1)
        //char temp[50];
        //scanf("%s", temp);
        //printf("%s", temp);
	printf("I LOVE YOU!");
     //   if (execl("B", "B", NULL) == -1)
      // fprintf(stderr, "B execl Error!\n");
       // exit(1);
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
