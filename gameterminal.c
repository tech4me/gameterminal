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
                if ((*(dir->d_name) == 'A' || *(dir->d_name) == 'B')&&(*(dir->d_name+1)!='.'))
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
    char a;
    pid_t pid;
    int rv;
    int commpipe1[2];// This holds the fd for the input & output of the pipe  Setup communication pipeline first 
    if (pipe(commpipe1))
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
        dup2(commpipe1[1], 1); // Replace stdout with out side of the pipe
        close(commpipe1[0]); // Close unused side of pipe (in side)
        setvbuf(stdout, (char*) NULL, _IONBF, 0);  // Set non-buffered output on stdout
        fprintf(stdout,"ab");
        wait(&rv);
    }
    else// A zero PID indicates that this is the child process 
    {
        dup2(commpipe1[0], 0); // Replace stdin with the in side of the pipe
        close(commpipe1[1]); // Close unused side of pipe (out side) Replace the child fork with a new process
        {
        if (execl("A", "A", NULL) == -1)
            fprintf(stderr, "execl Error!");
            exit(1);
        }
    }
}

int pfork(int);

int main(void) {
    int parentA[2];// This holds the fd for the input & output of the pipe  Setup communication pipeline first 
    int childA[2];
    
    int parentB[2];
    int childB[2];
    
    char buff[50];
    
    if (pipe(parentA) || pipe(childB) || pipe(parentA) ||pipe(childB) )
    {
        fprintf(stderr, "Pipe error!\n");
        exit(1);
    }
    
    int i;
    i = pfork(3);
    
    if (i == 0)// A positive (non-negative) PID indicates the parent process    // parent
    {
        const char love[] = "love your sons!\n";
        
        int inA, outA;
        inA = parentA[0];
        outA = childA[1];
        
        int inB, outB;
        inB = parentB[0];
        outB = childB[1];
        
        
        
        read (inA, buff, 50);
        write (outB, buff,strlen(buff) + 1);
        
        /*dup2(commpipeAParent[0], 0); // Replace stdin with the in side of the pipe
        close(commpipeAParent[1]); // Close unused side of pipe (out side) Replace the child fork with a new process
        {
        if (execl("A", "A", NULL) == -1)
            fprintf(stderr, "execl Error!");
            exit(1);
        }*/
    } else if (i==1)    // child A
     // A zero PID indicates that this is the child process 
    {
        char const hi[] = "hi\n";
        
        int in, out;
        in = childA[0];
        out = parentA[1];
        
      
        write (out,hi,strlen(hi)+1);
        //read(in, buff, 50);
        
       /* dup2(commpipeAParent[1], 1); // Replace stdout with out side of the pipe
        close(commpipeAParent[0]); // Close unused side of pipe (in side)
        setvbuf(stdout, (char*) NULL, _IONBF, 0);  // Set non-buffered output on stdout
        fprintf(stdout,);
        wait(&rv);
    */
    } else if (i==2) {  // child B
        char const hi[] = "Not\n";
        
        int in, out;
        in = childB[0];
        out = parentB[1];
        
        read(in, buff, 50);
       // write(out, hi, strlen(hi)+1)
        printf("%s",buff);
        
    }
    
    
    return 0;
}

int pfork(int x)
{
        int j;
        for(j=1;j<x;j++)
        {
                if(fork()==0)
                {
                    //sleep(3);
                    return j;
                }
        }
        return 0;
}
