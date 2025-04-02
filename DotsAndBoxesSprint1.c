#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define ROWS 5
#define COLS 6


char board[ROWS * 2 - 1][COLS * 2 - 1];  
int scoreA = 0, scoreB = 0;  
bool vsBot = false; //for the bot
int botDifficulty = 1;   

void initializeBoard() {
    for (int i = 0; i < ROWS * 2 - 1; i++) {
        for (int j = 0; j < COLS * 2 - 1; j++) {
            if (i % 2 == 0) {
                board[i][j] = (j % 2 == 0) ? '.' : ' ';  // Dots on even rows
            } else {
                board[i][j] = ' ';  // Empty space for lines
            }
        }
    }
}

void displayBoard() {

    printf("  ");
    for (int i = 0; i < COLS; i++) printf("%d ", i);
    printf("\n");

    for (int i = 0; i < ROWS * 2 - 1; i++) {
        if (i % 2 == 0) printf("%d ", i / 2);
        else printf("  ");

        for (int j = 0; j < COLS * 2 - 1; j++) {
            printf("%c", board[i][j]);
        }
        printf("\n");
    }
    
   
    
}


void placeMove(int r1, int c1, int r2, int c2) {
    if (r1 == r2) {  // Horizontal move
        if (c2 > c1) {
            board[r1 * 2][c1 * 2 + 1] = '-'; // draw the line to the right
        } else {
            board[r1 * 2][c2 * 2 + 1] = '-'; // draw the line to the left
        }
    } else {  // Vertical move
        if (r2 > r1) {
            board[r1 * 2 + 1][c1 * 2] = '|'; // to draw the line downwards
        } else {
            board[r2 * 2 + 1][c1 * 2] = '|'; // to draw the line upwards
        }
    }
}

bool checkForBoxes(char player) {
    bool boxCompleted = false;

    for (int i = 1; i < ROWS * 2 - 1; i += 2) {
        for (int j = 1; j < COLS * 2 - 1; j += 2) {
            if (board[i][j] == ' ' &&
                board[i - 1][j] == '-' &&
                board[i + 1][j] == '-' &&
                board[i][j - 1] == '|' &&
                board[i][j + 1] == '|') {
                
                board[i][j] = player;  // Place player's letter inside the box
                if (player == 'A') scoreA++;
                else scoreB++;
                boxCompleted = true;
            }
        }
    }

    return boxCompleted;
}
 // check if points are valid
 bool isValidMove(int r1, int c1, int r2, int c2) {
    if (r1 < 0 || r1 >= ROWS || c1 < 0 || c1 >= COLS ||
        r2 < 0 || r2 >= ROWS || c2 < 0 || c2 >= COLS) {
        return false;  // Out of bounds
    }

    if ((r1 == r2 && abs(c1 - c2) == 1) || (c1 == c2 && abs(r1 - r2) == 1)) {
        int row1 = r1 * 2, col1 = c1 * 2;
        int row2 = r2 * 2, col2 = c2 * 2;

        if (board[(row1 + row2) / 2][(col1 + col2) / 2] == ' ') {
            return true;  // Valid move (line not already placed)
        }
    }
    return false;
}
bool isGameOver() {
    return (scoreA + scoreB == (ROWS - 1) * (COLS - 1));
}

        //BOTMOVE
void botMove(int *r1, int *c1, int *r2, int *c2, int difficulty) {
    if (difficulty == 1) {  
        // Easy bot: Makes random moves
        int valid = 0;
        while (!valid) {
            *r1 = rand() % ROWS;
            *c1 = rand() % COLS;
            if (rand() % 2) {  
                *r2 = *r1;
                *c2 = (*c1 == COLS - 1) ? *c1 - 1 : *c1 + 1;
            } else {  
                *c2 = *c1;
                *r2 = (*r1 == ROWS - 1) ? *r1 - 1 : *r1 + 1;
            }
            valid = isValidMove(*r1, *c1, *r2, *c2);
        }
    }
     else if (difficulty == 2) {
        for (int i = 1; i < ROWS * 2 - 1; i += 2) {
            for (int j = 1; j < COLS * 2 - 1; j += 2) {
                if (board[i][j] == ' ') {
                    int count = 0;
                    int missingR = -1, missingC = -1;

                    if (board[i - 1][j] != '-') { count++; missingR = i - 1; missingC = j; }
                    if (board[i + 1][j] != '-') { count++; missingR = i + 1; missingC = j; }
                    if (board[i][j - 1] != '|') { count++; missingR = i; missingC = j - 1; }
                    if (board[i][j + 1] != '|') { count++; missingR = i; missingC = j + 1; }

                    if (count == 1) {
                        if (missingR % 2 == 0) {
                            *r1 = missingR / 2;
                            *r2 = *r1;
                            *c1 = (missingC - 1) / 2;
                            *c2 = *c1 + 1;
                        } else {
                            *c1 = missingC / 2;
                            *c2 = *c1;
                            *r1 = (missingR - 1) / 2;
                            *r2 = *r1 + 1;
                        }

                        if (isValidMove(*r1, *c1, *r2, *c2)) return;
                    }
                }
            }
        }
     }
}
void twoPlayers(){
    initializeBoard();
    char currentPlayer = 'A';
    
    while (!isGameOver()) {
    
        displayBoard();
        
        int r1, c1, r2, c2;
        bool extraTurn = false;
        bool validMove = false;

        do {
            printf("Player %c's turn. Enter row & column of first dot(e.g., A0-> 0 0, and then second dot: ", currentPlayer);
            scanf("%d %d %d %d", &r1, &c1, &r2, &c2);

            if (!isValidMove(r1, c1, r2, c2)) {
                printf("Invalid move! Try again.\n");
            } else {
                validMove = true;
                placeMove(r1, c1, r2, c2);
                extraTurn = checkForBoxes(currentPlayer);
                printf("***********************\n");
                printf("Player A scores: %d \nPlayer B scores: %d\n", scoreA, scoreB);
                printf("***********************\n");
            }
            
        } while (!validMove);

        if (!extraTurn) {
            currentPlayer = (currentPlayer == 'A') ? 'B' : 'A';
        }
    }

    displayBoard();
    printf("***********************\n");
    printf("Final Score :\nPlayer A: %d \nPlayer B: %d\n", scoreA, scoreB);
    printf("Winner: %s\n", (scoreA > scoreB) ? "Player A" : (scoreB > scoreA) ? "Player B" : "Draw!");
    printf("***********************\n");

    
}

void playWithBot() {
    initializeBoard();
    char currentPlayer = 'A';  // Player A is the user, substituted player B with the BOT 'B'

    while (!isGameOver()) {
        displayBoard();

        int r1, c1, r2, c2;
        bool extraTurn = false;
        bool validMove = false;

        if (currentPlayer == 'A') {  
            
            do {
                printf("Player A's turn. Enter row & column of first dot (e.g., A0-> 0 0, and then second dot): ");
                scanf("%d %d %d %d", &r1, &c1, &r2, &c2);

                if (!isValidMove(r1, c1, r2, c2)) {
                    printf("Invalid move! Try again.\n");
                } else {
                    validMove = true;
                    placeMove(r1, c1, r2, c2);
                    extraTurn = checkForBoxes('A');
                }

            } while (!validMove);
        } else {  
            
            printf("Bot is making a move...\n");
            botMove(&r1, &c1, &r2, &c2, botDifficulty);
            placeMove(r1, c1, r2, c2);
            extraTurn = checkForBoxes('B');
        }

        printf("***********************\n");
        printf("Player A scores: %d \nBot scores: %d\n", scoreA, scoreB);
        printf("***********************\n");

        if (!extraTurn) {
            currentPlayer = (currentPlayer == 'A') ? 'B' : 'A';
        }
    }

    displayBoard();
    printf("***********************\n");
    printf("Final Score:\nPlayer A: %d \nBot: %d\n", scoreA, scoreB);
    printf("Winner: %s\n", (scoreA > scoreB) ? "Player A" : (scoreB > scoreA) ? "Bot" : "Draw!");
    printf("***********************\n");
}

int main() {
   int choice;
    
    printf("Hello!! Choose your game mode:\n");
    printf("1. Multiplayer\n");
    printf("2. Play against Bot\n");
    printf("Enter your choice (1 or 2): ");
    scanf("%d", &choice);

    if (choice == 1) {
        twoPlayers();
    } else if (choice == 2) {
        printf("Choose bot difficulty:\n");
        printf("1. Easy\n");
        printf("2. Medium\n");
        printf("3. Hard\n");
        printf("Enter difficulty level (1-3): ");
        scanf("%d", &botDifficulty);

        vsBot = true;
        playWithBot();
        
    } else {
        printf("Invalid choice! Exiting program.\n");
    }

    return 0;
}
