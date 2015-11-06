/*
 * File:   main.c
 * Author: tech4me
 * Description: A program that takes two board game programs and execicute them
 * find the winner.(Have program "A" and "B" under the same directory as this
 * program.)
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <inttypes.h>
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h>

int pfork(int x);
bool checkFile();
void runPrograms(char colourA, char colourB, int n);
int readInA (int boardSize, char colour, char *buffer, int *outB, int *outA);
int readInB (int boardSize, char colour, char *buffer, int *outA, int *outB);

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
        runPrograms(colourA, colourB, boardSize);
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

void runPrograms(char colourA, char colourB, int n)
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
        sleep(1);
        read (inA, buffA, 50);
        read (inB, buffB, 50);
        printf("%s%s\n", buffA, buffB);
        //write (outB, buff,strlen(buff) + 1);
        int result, counter;
        if ( colourA == 'B' )
            counter = 1;
        else
            counter = 2;
        char buffer[100];
        do
        {
            result = 2;
            if( counter % 2 != 0 )
            {
                read(inA, buffer, 100);
                result = readInA (n, colourA, buffer, &outB, &outA);
            }
            else
            {
                read(inB, buffer, 100);
                readInB (n, colourB, buffer, &outA, &outB);
            }
            counter++;
        } while( result != 1 || result != 0 || result != -1 );
        printf("%d", result);
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

        if ( execl("A", "A", NULL) == -1 )
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

        if ( execl("B", "B", NULL) == -1 )
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

int readInA (int boardSize, char colour, char *buffer, int *outB, int *outA )    // A wins = 1, B wins = -1, Draw = 0
{
    char temp[2];
    if ( buffer[25] == ':' )
    {
        write (*outA, &boardSize, 3);    // CHANGE '3'
    }
    else if ( buffer[21] == ':' )
    {
        write (*outA, &colour, 3);    // CHANGE '3'
    }
    /*else if ( buffer[32] == ':' )
    {
        temp[0] = buffer[34];
        temp[1] = buffer[35];
        write(*outB, temp, 2);
    }*/
    else if (( buffer[18] == 'a' ) && ( buffer[19] == 't' ))
    {
        temp[0] = buffer[21];
        temp[1] = buffer[22];
        write(*outB, temp, 2);
    }
    else if ( buffer[13] == '.' )
    {
        if ( buffer[0] == colour )
            return 1;    // Program A wins
        else
            return -1;    // Program B wins
    }
    else if ( buffer[4] == '!' )
    {
        return 0;    // Draw
    }

}

// Have a loop outside of readIn()



int readInB (int boardSize, char colour, char *buffer, int *outA, int *outB)
{
    char temp[2];
    if ( buffer[25] == ':' )
    {
        write (*outB, &boardSize, 3);    // CHANGE '3'
    }
    else if ( buffer[21] == ':' )
    {
        write (*outB, &colour, 3);    // CHANGE '3'
    }
    /*else if ( buffer[32] == ':' )
    {
        temp[2];
        temp[0] = buffer[34];
        temp[1] = buffer[35];
        write(*outA, temp, 2);
    }*/
    else if (( buffer[18] == 'a' ) && ( buffer[19] == 't' ))
    {
        temp[0] = buffer[21];
        temp[1] = buffer[22];
        write(*outA, temp, 2);
    }
}
