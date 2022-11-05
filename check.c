#include <stdio.h>
#include "project.h"




int checkWin(int board[9])
{
    // Check Rows for Horizontal wins 
    for (int i = 0; i < 3; i++)
    {
        if (board[3 * i] == board[3 * i + 1] && board[3 * i + 1] == board[3 * i + 2])
        {
            return board[3 * i];
        }
    }

    // Check Columns for Vertical wins 
    for (int j = 0; j < 3; j++)
    {
        if (board[j] == board[3 + j] && board[3 + j] == board[6 + j])
        {
            return board[j];
        }
    }

    // Check for Diagonal wins
    if (board[0] == board[4] && board[0] == board[8])
    {
        return board[0];
    }

    if (board[2] == board[4] && board[2] == board[6])
    {
        return board[2];
    }

    return 0;
}