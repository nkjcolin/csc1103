#include <stdlib.h>
#include "project.h"


// imperfect minimax using depth-limited search
int impMiniMax(int board[9], int depth, int alpha, int beta, int maxTurn)
{
    int gameState = checkWin(board);
    int checkDraw = movesLeft(board);
    int limit = 2; // depth limit

    if(gameState == 1) // bot wins
    {
        return 10;
    }

    if (gameState == -1) // player wins
    {
        return -10;
    }
    if (checkDraw == 0) // draw
    {
        return 0;
    }

    if(maxTurn)
    {
        // maximizer's turn, get the highest score recursively
        int bestScore = MIN_SCORE;
        // checks every row and column for empty space
        for(int i = 0; i < 9; i++)
        {
            if(board[i] == 0 && depth < limit)
            {
                board[i] = 1;
                int tempScore = impMiniMax(board, depth + 1, alpha, beta, 0);
                board[i] = 0; // resets space to empty
                bestScore = max(bestScore, tempScore);
                if (bestScore >= beta)
                    break;
                alpha = max(alpha, bestScore);
            }
        }
        return bestScore;
    }

    else
    {
        // minimizer's turn, get the lowest score recursively
        int bestScore = MAX_SCORE;
        // checks every row and column for empty space
        for (int i = 0; i < 9; i++)
        {
            if (board[i] == 0 && depth < limit)
            {
                board[i] = -1;
                int tempScore = impMiniMax(board, depth + 1, alpha, beta, 1);
                board[i] = 0; // resets space to empty
                bestScore = min(tempScore, bestScore);
                if (bestScore <= alpha)
                    break;
                beta = min(beta, bestScore);
            }
        }
        return bestScore;
    }
}

// next best move imperfect minimax
// returns position of next move
int impMove(int board[9])
{
    int score = MIN_SCORE;
    int move = 0;

    for (int i = 0; i < 9; i++)
    {
        if (board[i] == 0)
        {
            board[i] = 1; // makes temporary move
            int tempScore = impMiniMax(board, 0, MIN_SCORE, MAX_SCORE, 0);
            board[i] = 0; // undo move
            if (tempScore > score)
            {
                score = tempScore;
                move = i;
            }
        }
    }
    board[move] = 1;
    return move; // return best possible position
}

// gauge difficulty level based on number of bot wins
int levelDifficulty(float botWins, float gamesPlayed)
{
    // if draw, botWins + 0.5; if bot wins, botWins + 1
    float difficulty = botWins / gamesPlayed;
    if (gamesPlayed != 0) // played at least 1 game
    {
        if (difficulty > 0.6) // Difficulty: Hard
            return 3;
        else if (difficulty > 0.4) // Difficulty: Intermediate
            return 2;
        else // Difficulty: Easy
            return 1;
    }
    return 0; // no games played yet
}