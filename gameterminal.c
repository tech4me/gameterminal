/* 
 * File:   main.c
 * Author: tech4me
 *
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

bool checkFile();
void runPrograms();

int main(int argc, char* argv[])//"boardsize" "BW or WB" BW= AB BW | WB= AW BB 
{
    int boardSize;
    char* endPtr;
    boardSize = strtoimax(argv[1], &endPtr, 10);
    char colourA, colourB;
    colourA = *argv[2];
    colourB = *(argv[2] + 1);

    if (checkFile())
    {
        runPrograms();
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
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            if (dir->d_type == DT_REG)
            {
                if (*(dir->d_name) == 'A' || *(dir->d_name) == 'B')
                    counter++;
                //printf("%s\n", dir->d_name);
            }
        }
        closedir(d);
    }
    if (counter == 2)
        return true;
    else
        return false;
}

void runPrograms()
{
    pid_t pid;
    int rv;
    int commpipe[2];// This holds the fd for the input & output of the pipe  Setup communication pipeline first 
    if (pipe(commpipe))
    {
        fprintf(stderr, "Pipe error!\n");
        exit(1);
    }
    if ((pid = fork()) == -1)// Attempt to fork and check for errors
    {
        fprintf(stderr, "Fork error. Exiting.\n");
        exit(1); // something went wrong
    }
    if (pid)// A positive (non-negative) PID indicates the parent process
    {
        dup2(commpipe[1], 1); // Replace stdout with out side of the pipe
        close(commpipe[0]); // Close unused side of pipe (in side)
        setvbuf(stdout, (char*) NULL, _IONBF, 0);  // Set non-buffered output on stdout
        sleep(2);
        printf("Hello\n");
        sleep(2);
        printf("Goodbye\n");
        sleep(2);
        printf("exit\n");
        wait(&rv);// Wait for child process to end
        fprintf(stderr, "Child exited with a %d value\n", rv); 
    }
    else// A zero PID indicates that this is the child process 
    {
        dup2(commpipe[0], 0); // Replace stdin with the in side of the pipe
        close(commpipe[1]); // Close unused side of pipe (out side) Replace the child fork with a new process
        {
        if (execl("./A", "./A", NULL) == -1)
            fprintf(stderr, "execl Error!");
            exit(1);
        }
    }
}
