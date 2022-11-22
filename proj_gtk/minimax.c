#include <stdlib.h>
#include "project.h"

// AI calculates best move using miniMax function and performs optimal move
int AImove(int board[9]) // update to return position of where to put the move
{
    int score = MIN_SCORE;
    int move = 0;

    for (int i = 0; i < 9; i++)
    {
        if (board[i] == 0)
        {
            board[i] = 1;                         // Temporarily set grid to AI's
            int tempScore = miniMax(board, 0, 0); // Calls miniMax recursive function to get best scoring move
            board[i] = 0;                         // Resets back grid
            if (tempScore > score)
            {
                score = tempScore;
                move = i;
            }
        }
    }
    board[move] = 1;
    return move;
}

// Checks all possible move in board and return value of board
// If AI wins, +10. If player wins, -10. If tie, 0. Otherwise, continue to find move values
int miniMax(int board[9], int depth, int maxTurn)
{
    int gameState = checkWin(board);
    int checkDraw = movesLeft(board);

    if (gameState == 1) // AI wins
    {
        return 10;
    }

    if (gameState == -1) // Player wins
    {
        return -10;
    }

    if (checkDraw == 0) // Tie
    {
        return 0;
    }

    if (maxTurn) // If maximizer's turn, get the highest score as possible recursively
    {
        int bestScore = MIN_SCORE;
        for (int i = 0; i < 9; i++) // Checks every row and column if empty box exists
        {
            if (board[i] == 0)
            {
                board[i] = 1;                                 // Temporarily set grid to AI's
                int tempScore = miniMax(board, depth + 1, 0); // Passes recursive function to minimizer's loop
                board[i] = 0;                                 // Resets back grid
                bestScore = max(tempScore, bestScore);
            }
        }
        return bestScore;
    }

    else // If minimizer's turn, get the lowest score as possible recursively
    {
        int bestScore = MAX_SCORE;
        for (int i = 0; i < 9; i++) // Checks every row and column if empty box exists
        {
            if (board[i] == 0)
            {
                board[i] = -1;                                // Temporarily set grid to player's
                int tempScore = miniMax(board, depth + 1, 1); // Passes recursive function to maximizer's loop
                board[i] = 0;                                 // Resets back grid
                bestScore = min(tempScore, bestScore);
            }
        }
        return bestScore;
    }
}

// Check if board has any more spaces left to fill
int movesLeft(int board[9])
{
    for (int i = 0; i < 9; i++)
    {
        if (board[i] == 0)
        {
            return 1;
        }
    }
    return 0;
}

// Returns higher value
int max(int a, int b)
{
    return a > b ? a : b;
}

// Returns mower value
int min(int a, int b)
{
    return a < b ? a : b;
}
