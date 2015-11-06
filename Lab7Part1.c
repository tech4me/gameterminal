#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

static const int MAX_BOARD_SIZE=26;
static const int DELTA[8][2]={-1,-1,-1,0,-1,1,0,-1,0,1,1,-1,1,0,1,1};

void boardInitialize(char board[MAX_BOARD_SIZE][MAX_BOARD_SIZE], int n);
void printBoard(char board[MAX_BOARD_SIZE][MAX_BOARD_SIZE], int n);
int legalCases(char board[MAX_BOARD_SIZE][MAX_BOARD_SIZE], char i, char j, char bwType, int n);
void userMove(char board[MAX_BOARD_SIZE][MAX_BOARD_SIZE], int n, char colour);
void moveFlip(char board[MAX_BOARD_SIZE][MAX_BOARD_SIZE], int n, char row, char col, char colour);
bool positionInBounds(int N,char row, char col);
int checkLegalInDirection(char board[MAX_BOARD_SIZE][MAX_BOARD_SIZE], int N, char row, char col, char colour, int deltaRow, int deltaCol);
char computerColour(char board[MAX_BOARD_SIZE][MAX_BOARD_SIZE], int n);
void gameProcess(char board[MAX_BOARD_SIZE][MAX_BOARD_SIZE], int n, char colour);
void computerMove(char board[MAX_BOARD_SIZE][MAX_BOARD_SIZE], int n, char colour);
bool checkWin(char board[MAX_BOARD_SIZE][MAX_BOARD_SIZE], int n, char colour);

int main(void)
{
    char board[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
    int boardSize;
    bool comColour;
    printf("Enter the board dimension: ");
    scanf("%d",&boardSize);
    boardInitialize(board,boardSize);    
    gameProcess(board,boardSize,computerColour(board,boardSize));       
}

void boardInitialize(char board[MAX_BOARD_SIZE][MAX_BOARD_SIZE], int n)//initialize stardard starting board configuration
{
    int i,j,k,m;
    for(i=0;i<MAX_BOARD_SIZE;i++)
    {
        for(j=0;j<MAX_BOARD_SIZE;j++)
            board[i][j]='\0';
    }
    for(k=0;k<n;k++)
    {
        for(m=0;m<n;m++)
            board[k][m]='U';
    }
    board[(n/2)-1][(n/2-1)]='W';
    board[(n/2)-1][n/2]='B';
    board[n/2][(n/2)-1]='B';
    board[n/2][n/2]='W';
}

void printBoard(char board[MAX_BOARD_SIZE][MAX_BOARD_SIZE], int n)//print current board
{
    int i,j,k;
    printf("  ");
    for(i=0;i<n;i++)
        printf("%c", 'a'+i);
    printf("\n");
    for(j=0;j<n;j++)
    {
        printf("%c ",'a'+j);
        for(k=0;k<n;k++)
            printf("%c",board[j][k]);
        printf("\n");
    }
    
}

char computerColour(char board[MAX_BOARD_SIZE][MAX_BOARD_SIZE], int n)
{
    char colour;
    printf("Computer plays (B/W) : ");
    scanf(" %c", &colour);
    printBoard(board, n);
    return colour;
}

int legalCases(char board[MAX_BOARD_SIZE][MAX_BOARD_SIZE], char row, char col , char colour, int n)
{
    int i;
    int result=0;
    for(i=0;i<8;i++)
    {
        result=result+checkLegalInDirection(board,n,row,col,colour,DELTA[i][0],DELTA[i][1]);
    }
    return result;
}

bool positionInBounds(int N, char row, char col)//helper func
{
    if((row>='a')&&(row<='a'+N-1)&&(col>='a')&&(col<='a'+N-1))
        return true;
    else
        return false;
}


int checkLegalInDirection(char board[MAX_BOARD_SIZE][MAX_BOARD_SIZE], int N, char row, char col, char colour, int deltaRow, int deltaCol)
{
    if(board[row-'a'][col-'a']!='U')
        return false;
    else
    {
        char bwType;
        if(colour=='B')
            bwType='W';
        else if(colour=='W')
            bwType='B';
        int i,counter=0;
        if(!positionInBounds(N,row+deltaRow,col+deltaCol))
            return 0;
        for(i=0;positionInBounds(N,row+deltaRow,col+deltaCol);i++)
        {
            if(board[row-'a'+deltaRow][col-'a'+deltaCol]==bwType)
            {
                counter++;
                row=row+deltaRow;
                col=col+deltaCol;
            }
            else if(board[row-'a'+deltaRow][col-'a'+deltaCol]==colour && counter<1)
                return 0;
            else if(board[row-'a'+deltaRow][col-'a'+deltaCol]==colour && counter>=1)
                return counter;
            else if(board[row-'a'+deltaRow][col-'a'+deltaCol]=='U')
                return 0;
        } 
        if(board[row-'a'][col-'a']==bwType && counter>=1)
            return 0;       
    }    
}

void userMove(char board[MAX_BOARD_SIZE][MAX_BOARD_SIZE], int n, char colour)
{
    int i;
    bool caseState=false;
    char userInput[3];
    char bwType;
    if(colour=='W')
        bwType='B';
    else if(colour=='B')
        bwType='W';
    printf("Make move for colour %c (Row/Col): ", colour);
    for(i=0;i<2;i++)
        scanf(" %c", &userInput[i]);
    if(positionInBounds(n, userInput[0], userInput[1]))
    {
        if(legalCases(board, userInput[0], userInput[1], colour, n)!=0)
            caseState=true;
        else
            caseState=false;
    }
    else
        caseState=false;
    if(caseState)
        moveFlip(board, n, userInput[0], userInput[1], colour);
    else
    {
        printf("Invalid move.\n");
        printf("%c player wins.\n",bwType);
        exit(0);
    }
}

void moveFlip(char board[MAX_BOARD_SIZE][MAX_BOARD_SIZE], int n, char row, char col, char colour)
{
    int i,j,counter=0;
    char bwType;
    char rowTemp;
    char colTemp;
    board[row-'a'][col-'a']=colour;
    if(colour=='W')
        bwType='B';
    else if(colour=='B')
        bwType='W';
    for(i=0;i<8;i++)
    {
        rowTemp=row;
        colTemp=col;
        counter=0;
        while(positionInBounds(n,rowTemp+DELTA[i][0],colTemp+DELTA[i][1]))
        {
            if(board[rowTemp+DELTA[i][0]-'a'][colTemp+DELTA[i][1]-'a']==bwType)
            {
                counter++;
                rowTemp=rowTemp+DELTA[i][0];
                colTemp=colTemp+DELTA[i][1];
            }            
            else if(board[rowTemp+DELTA[i][0]-'a'][colTemp+DELTA[i][1]-'a']=='U')
                break;
            else if(board[rowTemp+DELTA[i][0]-'a'][colTemp+DELTA[i][1]-'a']==colour&&counter<1)
                break;
            else if(board[rowTemp+DELTA[i][0]-'a'][colTemp+DELTA[i][1]-'a']==colour&&counter>=1)
            {
                for(j=0;j<counter;j++)
                    board[rowTemp-j*DELTA[i][0]-'a'][colTemp-j*DELTA[i][1]-'a']=colour;
                break;
            }
        }
    } 
}

void gameProcess(char board[MAX_BOARD_SIZE][MAX_BOARD_SIZE], int n, char colour)
{
    bool gameState=true;
    while(gameState==true)
    {
        if(colour=='B')
        {
            if(checkWin(board,n,'B')==true)
            {
                computerMove(board,n,'B');
                printBoard(board,n);
            }
            if(checkWin(board,n,'W')==true)
            {
                userMove(board,n,'W');
                printBoard(board,n);
            }
        }
        else if(colour=='W')
        {
            if(checkWin(board,n,'B')==true)
            {
                userMove(board,n,'B');
                printBoard(board,n);
            }
            if(checkWin(board,n,'W')==true)
            {
                computerMove(board,n,'W');
                printBoard(board,n);
            }
        }
    }   
}

void computerMove(char board[MAX_BOARD_SIZE][MAX_BOARD_SIZE], int n, char colour)
{
    int i,j,counter=0;
    int temp[3];
    int bestMove[3]={0};
    for(i=0;i<n;i++)
    {
        for(j=0;j<n;j++)
        {
            temp[0]=i;
            temp[1]=j;
            temp[2]=legalCases(board,i+'a',j+'a',colour,n);
            if(temp[2]>bestMove[2])
            {
                bestMove[0]=temp[0];
                bestMove[1]=temp[1];
                bestMove[2]=temp[2];
            }
        }
    }
    moveFlip(board,n,bestMove[0]+'a',bestMove[1]+'a',colour);
    printf("Computer places %c at %c%c.\n",colour,bestMove[0]+'a',bestMove[1]+'a');
}

bool checkWin(char board[MAX_BOARD_SIZE][MAX_BOARD_SIZE], int n, char colour)
{
    int i,j,counterB=0,counterW=0,result=0;
    for(i=0;i<n;i++)
    {
        for(j=0;j<n;j++)
        {
            result=result+legalCases(board,i+'a',j+'a',colour,n);
            if(board[i][j]=='B')
                counterB++;
            else if(board[i][j]=='W')
                counterW++;
        }
    }
    if(counterB+counterW==n*n)//board full
    {
        if(counterB==counterW)
            printf("Draw!\n");
        else if(counterB>counterW)
            printf("B player wins.\n");
        else if(counterW>counterB)
            printf("W player wins.\n");
        exit(0);
    }
    else if(result==0)//no valid move
    {
        printf("%c player has no valid move.\n", colour);
        return false;
    }
    else
        return true;
}
