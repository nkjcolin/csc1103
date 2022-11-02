#include <stdio.h>
#include <math.h>
#include <stdlib.h>

// Function declaration
void placeX();
void placeO();
void playAgain();
void singleMode();
int displayMenu();
void normalMode();
char gridChar(int i); 
int max(int a, int b);
int min(int a, int b);
void AImove(int board[9]);
int checkWin(int board[9]);
int movesLeft (int board[9]);
void displayBoard(int board[9]);
int miniMax (int board[9], int depth, int maxTurn);


// Global variables
int choice = 0;
int restart = 0;
int board[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

int main()
{
    while(1)
    {
        displayMenu();

        while(choice == 1 || choice == 2 || choice == 3)
        {
            // set all square to ' '
            for(int i = 0; i < 9; i++) board[i] = 0;

            switch (choice)
            {
                case 1:
                singleMode();
                playAgain();
                break;

                case 2:
                normalMode();
                playAgain();
                break;

                case 3:
                break;

                default:
                break;
            }

            break;   
        }

        if(restart == 1);

        else
        {
            printf("Thank you and have a nice day!\n");
            return 0;
        }
    }
    return 0;
}

int displayMenu()
{
    // display menu
    printf("\nWelcome to Tic Tac Toe\n");
    printf("1. Single Player\n");
    printf("2. Two Player\n");
    printf("3. Exit\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    if (choice == 1 || choice == 2 || choice == 3)
    {
        return 1;
    }

    else
    {
        printf("Please choose 1, 2 or 3.\n");
        displayMenu();
    }
}

void displayBoard(int board[9])
{
    printf("\n       |       |       \n");
    printf("   %c   |   %c   |    %c  \n", gridChar(board[0]), gridChar(board[1]), gridChar(board[2]));
    printf("_______|_______|_______\n");
    printf("       |       |       \n");
    printf("   %c   |   %c   |    %c  \n", gridChar(board[3]), gridChar(board[4]), gridChar(board[5]));
    printf("_______|_______|_______\n");
    printf("       |       |       \n");
    printf("   %c   |   %c   |    %c  \n", gridChar(board[6]), gridChar(board[7]), gridChar(board[8]));
    printf("       |       |       \n");
}

// Inputs X or O based on whose turn it is else leave it blank
char gridChar(int i) 
{
    switch(i) 
    {
        case -1:
            return 'O';
        case 0:
            return ' ';
        case 1:
            return 'X';
    }
}

// 1 player mode against miniMax AI
void singleMode()
{
    int check, checkMoves = 1;
    int player = 1;
    unsigned turn = 1;

    // AI plays as X = 1, player plays as -1.
    printf("\nAI: X, You: O");
    printf("\nStart 1st or 2nd: ");
    scanf("%d", &player);


    while(checkMoves)
    {
        checkMoves = movesLeft(board);
        if(checkMoves != 1) 
        {
            displayBoard(board);
            printf("----------DRAW----------\n");
            break;
        }

        if((turn + player) % 2 == 0)
        {
            displayBoard(board);
            placeO();
            turn++;
            check = checkWin(board);

            if(check == -1) 
            {
                displayBoard(board);
                printf("Player O won.\n");
                break;
            }
        }

        else
        {
            AImove(board);
            turn++;
            check = checkWin(board);

            if(check == 1) 
            {
                displayBoard(board);
                printf("Player X won.\n");
                break;
            }
        }
    }
}

// AI calculates best move using miniMax function and performs optimal move
void AImove(int board[9])
{
    int score = -100;
    int move = 0;
    
    for(int i = 0; i < 9; i++)
    {
        if(board[i] == 0)
        {
            board[i] = 1;
            int tempScore = miniMax(board, 0, 0);
            board[i] = 0;
            if (tempScore > score)
            {
                score = tempScore;
                move = i;
            }
        }
    }
    board[move] = 1;
}

// 2 player mode
void normalMode()
{
    int check = 1;

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

// Put 1 which translates to X on the board
void placeX() 
{
    int a;

    //displayBoard(board);

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

    //displayBoard(board);
}

// Put -1 which translates to O on the board
void placeO()
{
    int b;

    //displayBoard(board);

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

    //displayBoard(board);
}

void playAgain()
{
    printf("\nDo you want to play again?\n");
    printf("1. Enter 1 to continue playing.\n");
    printf("Press any key exit: ");
    scanf("%d", &restart);
}

// Checks board inputs if there are any win conditions detected
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

// Checks all possible move in board and return value of board
// If 10, player wins. If -10, opponent wins. If 0, tie. Otherwise, calculate move values
int miniMax (int board[9], int depth, int maxTurn)
{
    int gameState = checkWin(board);
    int checkDraw = movesLeft(board);

    if(gameState == 1) 
    {
        return 10;
    }

    if(gameState == -1) 
    {
        return -10;
    }

    if (checkDraw == 0) // Tie
    {
        return 0;
    }
   
    if (maxTurn) // If maximizer's turn, get the highest score as possible recursively
    {
        int bestScore = -100;
        for (int i = 0; i < 9; i++) // Checks every row and column if empty box exists
        {
            if (board[i] == 0) 
            {
                board[i] = 1;
                int tempScore = miniMax(board, depth + 1, 0);
                board[i] = 0; // Resets back box
                bestScore = max(tempScore, bestScore);
            }
        }
        return bestScore;
    } 
    
    else // If minimizer's turn, get the lowest score as possible recursively
    {
        int bestScore = 100;
        for (int i = 0; i < 9; i++) // Checks every row and column if empty box exists
        {
            if (board[i] == 0)
            {
                board[i] = -1;
                int tempScore = miniMax(board, depth + 1, 1);
                board[i] = 0; // Resets back box
                bestScore = min(tempScore, bestScore);
            }
        }
        return bestScore;
    }
}

// Check if board has any more space left to fill
int movesLeft (int board[9])
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

// Returns lower value
int min(int a, int b) 
{
    return a < b ? a : b;
}