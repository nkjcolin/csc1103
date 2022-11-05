#include <stdio.h>
#include "project.h"

void displayBoard(int board[9])
{

    printf("       |       |       \n");
    printf("   %c   |   %c   |    %c  \n", board[0], board[1], board[2]);
    printf("_______|_______|_______\n");
    printf("       |       |       \n");
    printf("   %c   |   %c   |    %c  \n", board[3], board[4], board[5]);
    printf("_______|_______|_______\n");
    printf("       |       |       \n");
    printf("   %c   |   %c   |    %c  \n", board[6], board[7], board[8]);
    printf("       |       |       \n");
}

int displayMenu()
{
    int choice;

    //display menu
    printf("Welcome to tic tac toe\n");
    printf("1. Single Player\n");
    printf("2. Two Player\n");
    printf("3. EXIT\n");
    printf("Enter your choice: \n");
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