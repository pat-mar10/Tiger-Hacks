#include <stdio.h>
#define MAZE_SIZE 10

char convertLetters(char character){
    if(character >= 'a' && character <= 'z'){
        return character - 'a' + 'A';
    }
    return character;
}

void displayVisibleMaze(char maze[MAZE_SIZE][MAZE_SIZE], char visibleMaze[MAZE_SIZE][MAZE_SIZE], int playerX, int playerY, int hasCollectedSecretPoint, int gameWon){
    for(int i = 0; i < MAZE_SIZE; i++){
        for (int j = 0; j < MAZE_SIZE; j++){
            if(i == playerX && j == playerY && gameWon && maze[i][j] == 'E'){
                printf("x ");
            }else if(i == playerX && j == playerY){
                printf("x ");
            }else if(maze[i][j] == 'P' && !hasCollectedSecretPoint){
                printf("🌽 ");
            }else if (visibleMaze[i][j] == '1'){
                printf("👣 ");
            }else if(visibleMaze[i][j] == '~' || visibleMaze[i][j] == '1'){
                printf("🌽 ");
            }else{
                printf("%c ", visibleMaze[i][j]);
            }
        }
        printf("\n");
    }
    printf("\n");
}

int isValidMove(int x, int y, char maze[MAZE_SIZE][MAZE_SIZE], char visibleMaze[MAZE_SIZE][MAZE_SIZE]){
    return x >= 0 && x < MAZE_SIZE && y >= 0 && y < MAZE_SIZE && (maze[x][y] != '~' || visibleMaze[x][y] == '1');
}

int movePlayer(char direction, int *playerX, int *playerY, char maze[MAZE_SIZE][MAZE_SIZE], char visibleMaze[MAZE_SIZE][MAZE_SIZE], int startX, int startY, int endX, int endY, int tickets, int *hasCollectedSecretPoint, char *message){
    int newX = *playerX, newY = *playerY;
    direction = convertLetters(direction);
    
    switch(direction){
        case 'U': 
            newX--; 
        break;
        case 'D': 
            newX++; 
        break;
        case 'L': 
            newY--; 
        break;
        case 'R':
            newY++; 
        break;
        default:
            sprintf(message, "Invalid move! Use U, D, L, or R.");
        return tickets;
    }

    if(isValidMove(newX, newY, maze, visibleMaze)){
        *playerX = newX;
        *playerY = newY;

        if(maze[*playerX][*playerY] == 'P' && !*hasCollectedSecretPoint){
            tickets++;
            *hasCollectedSecretPoint = 1;
            sprintf(message, "You found the secret point! Tickets: %d", tickets);
            visibleMaze[*playerX][*playerY] = 'P';  
        }else if(maze[*playerX][*playerY] == 'P' && *hasCollectedSecretPoint){
            sprintf(message, "Secret point already collected.");
        }else{
            visibleMaze[*playerX][*playerY] = '1';  
            sprintf(message, "");
        }

        if(*playerX == endX && *playerY == endY){
            tickets++; 
            sprintf(message, "Congrats! You completed the maze! Total tickets: %d", tickets);
        }
    }else{
        sprintf(message, "Cannot move there, it's a wall.");
    }

    return tickets;
}

int main(){
    char maze[MAZE_SIZE][MAZE_SIZE] = {
        {'~', 'S', '~', '~', '~', '~', '~', '~', '~', '~'},
        {'~', '1', '1', '1', '~', '~', '~', '~', '~', '~'},
        {'~', '~', '1', '~', '~', '~', '~', '~', 'P', '~'},
        {'~', '~', '1', '1', '1', '1', '~', '~', '1', '~'},
        {'~', '~', '~', '~', '~', '1', '~', '~', '1', '~'},
        {'~', '~', '~', '~', '~', '1', '1', '1', '1', '~'},
        {'~', '~', '~', '~', '~', '~', '1', '~', '~', '~'},
        {'~', '~', '~', '1', '1', '1', '1', '~', '~', '~'},
        {'~', '~', '~', '1', '~', '~', '~', '~', '~', '~'},
        {'~', '~', '~', '1', '1', '1', '1', '1', '1', 'E'}
    };

    int startX = 0, startY = 1;
    int endX = 9, endY = 9;
    int playerX = startX, playerY = startY;
    int tickets = 0;
    int hasCollectedSecretPoint = 0;
    int gameWon = 0;
    char visibleMaze[MAZE_SIZE][MAZE_SIZE];
    char move;
    char message[100] = ""; 
    
    for(int i = 0; i < MAZE_SIZE; i++){
        for (int j = 0; j < MAZE_SIZE; j++){
            visibleMaze[i][j] = '~';
        }
    }
    visibleMaze[startX][startY] = 'S';
    visibleMaze[endX][endY] = 'E';

    printf("\033[4m🌽 Welcome to the Corn Maze!🌽\033[0m\nUse U (up), D (down), L (left), and R (right) to navigate.\nFind the secret point to earn bonus tickets!\n");

    while(!gameWon){
        displayVisibleMaze(maze, visibleMaze, playerX, playerY, hasCollectedSecretPoint, gameWon);

        if(message[0] != '\0'){
            printf("%s\n", message);
        }

        printf("Enter your move (U, D, L, R): ");
        scanf(" %c", &move);
        tickets = movePlayer(move, &playerX, &playerY, maze, visibleMaze, startX, startY, endX, endY, tickets, &hasCollectedSecretPoint, message);
        
        if(playerX == endX && playerY == endY){
            gameWon = 1;
        }
    }

    displayVisibleMaze(maze, visibleMaze, playerX, playerY, hasCollectedSecretPoint, gameWon);
    printf("Maze complete! Final tickets: %d\n", tickets);
}
