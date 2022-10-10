#include <stdio.h>
#include <math.h>
#include <stdlib.h>


void displayboard();
void displaymenu();
void playAgain();
int checkwinX();
int checkwinO();
void placeX();
void placeO();
void botmode();
void easybot();
void interbot();
void hardbot();

void singlemode(int bot);
void normalmode();


//global variables
int choice = 0;
int botchoice = 0;
int playagain = 0;
char board[8];
char win = ' ';
int option = 0;
int easyrand = 0;


int main()
{
    
    while(1)
    {
    displaymenu();

    while(choice == 1 || choice == 2 || choice == 3)
        {
                

            switch (choice)
            {
                case 1:
                botmode();
                    switch(botchoice)
                    {
                        case 1:
                        singlemode(botchoice);
                        break;
                    }
                //singlemode();
                playAgain();
                break;

                case 2:
                normalmode();
                playAgain();
                
                break;

                case 3:
                printf("Thank you and have a nice day!\n");
                break;

                default:
                printf("Please choose 1, 2 or 3.\n\n");
                break;
            }

            break;   



        }

        if(playagain == 1);
        
        else
        {
            break;
        }

    }
    return 0;
}


void singlemode(int bot)
{
    int check = 0;

    switch(bot)
    {
        case 1:

        for(int i=0; i<9; i++) board[i] = ' ';
        displayboard();
        
        for(int h=0;h<=9;h++)
        {
            placeO();

            check = checkwinO();

            if(check == 1)
            {
                printf("Player O won.\n");
                break;
            }
            h++;

            if(h == 9)
            {
                printf("--------DRAW--------\n");
                break;
            }
            printf("\n");
            easybot();

            check = checkwinX();

            if(check == 1)
            {
                printf("BOT won.\n");
                break;
            }

            

        }
        break;

        case 2:
        break;
    }


}


void easybot()
{
    easyrand = rand() % 9 + 1;



    if (easyrand >= 1 && easyrand <=9)
    {
        if(board[easyrand-1] == ' ')
        {
            board[easyrand-1] = 'X';
        }
        else
        {
        easybot();
        }
    }
    else if(easyrand >= 10 )
    {
    easybot();
    }

    displayboard();

}



void normalmode()
{
    
    int check = 0;

    //set all square to ' '
    for(int i=0; i<9; i++) board[i] = ' ';
    displayboard();

    for(int h=0;h<=9;h++)
    {
            //player X place
            placeX(); 

            //check if player x win
            check = checkwinX();

            if(check == 1)
            {
                printf("Player X won.\n");
                break;
            }

            h++;

            if(h == 9)
            {
                printf("--------DRAW--------\n");
                break;
            }

            //player O place
            placeO();

            //check if player O win
            check = checkwinO();

            if(check == 1)
            {
            printf("Player O won.\n");
            break;
            }
            
            
    }

}


void placeX()
{
    int a;

    //displayboard();

    printf("Player X, please choose 1~9 to place.\n");
    scanf("%d", &a);



    if (a >= 1 && a <=9)
    {
        if(board[a-1] == ' ')
        {
            board[a-1] = 'X';
        }
        else
        {
        printf("Spot taken, please enter other number\n");
        placeX();
        }
    }
    else if(a >= 10 )
    {
    printf("Player X, please choose 1~9 to place.\n");
    placeX();
    }

    displayboard();

    
    
    
}

void placeO()
{
    int b;

    //displayboard();

    printf("Player O, please choose 1~9 to place.\n");
    scanf("%d", &b);

    if (b >= 1 && b <=9)
    {
        if(board[b-1] == ' ')
        {
            board[b-1] = 'O';
        }
        else
        {
        printf("Spot taken, please enter other number\n");
        placeO();
        }
    }
    else if(b >= 10 )
    {
    printf("Player O, please choose 1~9 to place.\n");
    placeO();
    }

    displayboard();
    
}


void displayboard()
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

void displaymenu()
{
    //display menu
    printf("Welcome to tic tac toe\n");
    printf("1. Single Player\n");
    printf("2. Two Player\n");
    printf("3. EXIT\n");
    printf("Enter your choice: \n");
    scanf("%d", &choice);
}

void botmode()
{
    //display 3 mode
    printf("\n1. Easy");
    printf("\n2. Intermediate");
    printf("\n3. Hard");
    printf("\nPlease choose difficulty: ");
    scanf("%d", &botchoice);
}


void playAgain()
{
    printf("Do you want to play again?\n");
    printf("1. Enter 1 to continue playing.\n");
    printf("2. Press any key to stop playing.\n");
    scanf("%d", &playagain);

}

int checkwinX()
{

    //for horizontal wins
    if(board[0] == 'X' && board[1] == 'X' && board[2] == 'X')
    {
        return 1;
    }

    else if(board[3] == 'X' && board[4] == 'X' && board[5]== 'X')
    {
        return 1;
    }

    else if(board[6] == 'X' && board[7] == 'X' && board[8] == 'X')
    {
        return 1;
    }

    //for vertical wins
    else if(board[0] == 'X' && board[3] == 'X' && board[6] == 'X')
    {
        return 1;
    }

    else if(board[1] == 'X' && board[4] == 'X' && board[7] == 'X')
    {
        return 1;
    }

    else if(board[2] == 'X' && board[5] == 'X' && board[8] == 'X')
    {
        return 1;
    }

    //for diagonal wins
    else if(board[0] == 'X' && board[4] == 'X' && board[8] == 'X')
    {
        return 1;
    }

    else if(board[2] == 'X' && board[4] == 'X' && board[6] == 'X')
    {
        return 1;
    }

    return 0;
}

int checkwinO()
{

    //for horizontal wins
    if(board[0] == 'O' && board[1] == 'O' && board[2] == 'O')
    {
        return 1;
    }

    else if(board[3] == 'O' && board[4] == 'O' && board[5]== 'O')
    {
        return 1;
    }

    else if(board[6] == 'O' && board[7] == 'O' && board[8] == 'O')
    {
        return 1;
    }

    //for vertical wins
    else if(board[0] == 'O' && board[3] == 'O' && board[6] == 'O')
    {
        return 1;
    }

    else if(board[1] == 'O' && board[4] == 'O' && board[7] == 'O')
    {
        return 1;
    }

    else if(board[2] == 'O' && board[5] == 'O' && board[8] == 'O')
    {
        return 1;
    }

    //for diagonal wins
    else if(board[0] == 'O' && board[4] == 'O' && board[8] == 'O')
    {
        return 1;
    }

    else if(board[2] == 'O' && board[4] == 'O' && board[6] == 'O')
    {
        return 1;
    }

    return 0;
}

