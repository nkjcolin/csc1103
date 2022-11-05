#include <stdio.h>
#include "project.h"

void normalMode()
{
    int check = 1;
    int board[9];

    for(int turn = 1; turn <= 9; turn++)
    {
        // player X place
        displayBoard(board);
        printf("\nTurn %d. ", turn);
        placeX(); 

        // check if player x win
        check = checkWin(board);

        if(check == 1)
        {
            displayBoard(board);
            printf("Player X won.\n");
            break;
        }

        if(turn == 9)
        {
            displayBoard(board);
            printf("----------DRAW----------\n");
            break;
        }
        turn++;

        // player O place
        displayBoard(board);
        printf("\nTurn %d. ", turn);
        placeO();

        // check if player O win
        check = checkWin(board);

        if(check == -1)
        {
            displayBoard(board);
            printf("Player O won.\n");
            break;
        }
    }
}
