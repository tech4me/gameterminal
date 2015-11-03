#include <stdio.h> 

int main()
{
    char string[100];
    printf("Child Process\n");
    printf("-------------\n");
    do
    {
        printf("Enter Command: ");
        fflush(stdout);
        fgets(string, 100, stdin); // Must flush to see command prompt
        printf("%s\n", string); // No flush necessary because new line flushes
    }
    while (!strstr(string, "exit"));
    return 0;
}
