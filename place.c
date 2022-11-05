#include <stdio.h>
#include "project.h"
// Put 1 which translates to X on the board
void placeX() 
{
    int a;
    int board[9];
    

    printf("Player X, please choose 1 - 9 to place: ");
    scanf("%d", &a);

    if (a >= 1 && a <=9)
    {
        if(board[a-1] == 0)
        {
            board[a-1] = 1;
        }

        else
        {
            printf("\nSpot taken, please enter another number\n");
            placeX();
        }
    }

    else
    {
        printf("Player X, please choose 1 - 9 to place.\n");
        placeX();
    }

    
}

// Put -1 which translates to O on the board
void placeO()
{
    int b;
    int board[9];

    

    printf("Player O, please choose 1 - 9 to place: ");
    scanf("%d", &b);

    if (b >= 1 && b <=9)
    {
        if(board[b-1] == 0)
        {
            board[b-1] = -1;
        }
        else
        {
            printf("\nSpot taken, please enter another number.\n");
            placeO();
        }
    }

    else
    {
        printf("Player O, please choose 1 - 9 to place.\n");
        placeO();
    }

    
}

void playAgain()
{
    int restart;

    printf("\nDo you want to play again?\n");
    printf("1. Enter 1 to continue playing.\n");
    printf("Press any key exit: ");
    scanf("%d", &restart);
}