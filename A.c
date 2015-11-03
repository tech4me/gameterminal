#include <stdio.h> 

int main()
{
    int i;
    char input[2];
    printf("Child Process\n");
    printf("-------------\n");
    printf("Enter Command: \n");
    fflush(stdout);
    for(i=0;i<2;i++)
        scanf("%c",&input[i]);
        //fgets(string, 100, stdin); // Must flush to see command prompt
    printf("%c%c\n", input[0],input[1]); // No flush necessary because new line flushes
    return 0;
}
