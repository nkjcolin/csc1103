#define MIN_SCORE -100
#define MAX_SCORE 100

// Function declaration
int checkWin(int board[9]);
void placeX(int a);
void placeO(int b);
int max(int a, int b);
int min(int a, int b);
int AImove(int board[9]);
int impMove(int board[9]);
int movesLeft(int board[9]);
int miniMax(int board[9], int depth, int maxTurn);
int impMiniMax(int board[9], int depth, int alpha, int beta, int maxTurn);
int levelDifficulty(float botWins, float gamesPlayed);



// Global Variables
int board[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
int player = 0;
float gamesPlayed = 0;
float botWins = 0;


