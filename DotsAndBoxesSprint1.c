#include <stdio.h>
#include <stdbool.h>

#define ROWS 5
#define COLS 6

char grid[ROWS * 2 - 1][COLS * 2 - 1];
int scoreA = 0, scoreB = 0;
char currentPlayer = 'A';

void displayGrid() {
    for (int i = 0; i < ROWS * 2 - 1; i++) {
        for (int j = 0; j < COLS * 2 - 1; j++) {
            if (i % 2 == 0) {
                grid[i][j] = (j % 2 == 0) ? '.' : ' ';  // Dots on even rows
            } else {
                grid[i][j] = ' ';  // Empty space for lines
            }
        }
    }
}

void printGrid() {
    printf("  ");
    for (int i = 0; i < COLS; i++) printf("%d ", i);
    printf("\n");

    for (int i = 0; i < ROWS * 2 - 1; i++) {
        if (i % 2 == 0) printf("%d ", i / 2);
        else printf("  ");

        for (int j = 0; j < COLS * 2 - 1; j++) {
            printf("%c", grid[i][j]);
        }
        printf("\n");
    }
    
}

bool checkForBoxes() {
    bool boxCompleted = false;

    for (int i = 1; i < ROWS * 2 - 1; i += 2) {
        for (int j = 1; j < COLS * 2 - 1; j += 2) {
            if (grid[i][j] == ' ' && 
                grid[i - 1][j] == '-' && 
                grid[i + 1][j] == '-' && 
                grid[i][j - 1] == '|' && 
                grid[i][j + 1] == '|') {

                grid[i][j] = currentPlayer;  // Place the player's letter inside the box

                // Update the score
                if (currentPlayer == 'A') {
                    scoreA++;
                } else {
                    scoreB++;
                }

                boxCompleted = true;
            }
        }
    }

    return boxCompleted;
}

bool isValidMove(int r1, int c1, int r2, int c2) {
    int row = r1 + r2, col = c1 + c2;
    return (row >= 0 && row < ROWS * 2 - 1 && col >= 0 && col < COLS * 2 - 1 && grid[row][col] == ' ');
}

void drawLine(int r1, int c1, int r2, int c2) {
    if (r1 == r2) {
        grid[r1 * 2][c1 * 2 + 1] = '-';
    } else {
        grid[r1 * 2 + 1][c1 * 2] = '|';
    }
}

void playerMove() {
    int r1, c1, r2, c2;
    bool completedBox = false;

    while (1) {
        printf("Player %c's turn. Enter the row and column of the first dot (e.g., 0 0) and the second dot: ", currentPlayer);
        scanf("%d %d %d %d", &r1, &c1, &r2, &c2);
        printf("********************************\n");
        printf("Player A Score: %d\nPlayer B Score: %d\n", scoreA, scoreB);
        printf("********************************\n");

        if (isValidMove(r1, c1, r2, c2)) {
            drawLine(r1, c1, r2, c2);
            completedBox = checkForBoxes();
            printGrid();
            if (!completedBox) {
                currentPlayer = (currentPlayer == 'A') ? 'B' : 'A';
            }
            break;
        } else {
            printf("Invalid move! Please try again.\n");
        }
        
    }
}

bool isGameOver() {
    for (int i = 0; i < ROWS * 2 - 1; i++) {
        for (int j = 0; j < COLS * 2 - 1; j++) {
            if (grid[i][j] == ' ') {
                return false;  
            }
        }
    }
    return true;
}

int main() {
    displayGrid();
    printGrid();
    
    while (!isGameOver()) {
        
        playerMove();
        
    }
    
    printGrid();
    printf("********************************\n");
    printf("Final Score :\nPlayer A: %d \nPlayer B: %d\n", scoreA, scoreB);
    printf("Winner: %s\n", (scoreA > scoreB) ? "Player A" : (scoreB > scoreA) ? "Player B" : "Draw!");
    printf("********************************\n");
    return 0;
}