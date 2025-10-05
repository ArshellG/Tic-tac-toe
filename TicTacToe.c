#include <stdio.h>
#include <stdlib.h>
#include <time.h>


char **boardInitialize(int N){
    char **board = malloc(N * sizeof *board);
    for (int i = 0; i < N; i++) {
        board[i] = malloc(N * sizeof(char));
    }
    return board;
}

void boardCreate(int N, char **board, char fill){
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            board[i][j] = fill;
}

void boardFree(int N, char **board){
    for (int i = 0; i < N; i++)
        free(board[i]);
    free(board);
}

void PrintBoard(int N, char **board){
    printf("   ");
    for (int c = 0; c < N; c++) printf("%3d ", c);
    printf("\n   ");
    for (int c = 0; c < N; c++) printf("----");
    printf("-\n");
    for (int r = 0; r < N; r++) {
        printf("%2d ", r);
        for (int c = 0; c < N; c++)
            printf("| %c ", board[r][c]);
        printf("|\n   ");
        for (int c = 0; c < N; c++) printf("----");
        printf("-\n");
    }
}


int PlayerMove(int *row, int *col, char player){
    printf("Player %c, enter row and column (e.g., 1 2): ", player);
    if (scanf("%d %d", row, col) != 2) {
        int ch; while ((ch = getchar()) != '\n' && ch != EOF) {}
        return 0;
    }
    return 1;
}

int CellEmpty(char **board, int r, int c){
    return board[r][c] == ' ';
}

int WinCheck(char **board, int N, char player){
    // Rows
    for (int i = 0; i < N; i++) {
        int rowWin = 1;
        for (int j = 0; j < N; j++)
            if (board[i][j] != player) { rowWin = 0; break; }
        if (rowWin) return 1;
    }

    // Columns
    for (int j = 0; j < N; j++) {
        int colWin = 1;
        for (int i = 0; i < N; i++)
            if (board[i][j] != player) { colWin = 0; break; }
        if (colWin) return 1;
    }

    // Main diagonal
    int mainDiag = 1;
    for (int i = 0; i < N; i++)
        if (board[i][i] != player) { mainDiag = 0; break; }
    if (mainDiag) return 1;

    // Anti-diagonal
    int antiDiag = 1;
    for (int i = 0; i < N; i++)
        if (board[i][N - 1 - i] != player) { antiDiag = 0; break; }
    if (antiDiag) return 1;

    return 0;
}

int DrawCheck(char **board, int N){
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (board[i][j] == ' ') return 0;
    return 1;
}

int ComputerMoves(char **board, int N, int *r, int *c){
    int freeSpots[100][2];
    int pos = 0;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (board[i][j] == ' ') {
                freeSpots[pos][0] = i;
                freeSpots[pos][1] = j;
                pos++;
            }

    if (pos == 0) return 0;
    int pick = rand() % pos;
    *r = freeSpots[pick][0];
    *c = freeSpots[pick][1];
    return 1;
}
FILE *openLog(const char *path, const char *mode){
    FILE *fp = fopen(path, mode);
    if (!fp) perror("log file");
    return fp;
}

void logMove(FILE *fp, int move_no, char player, int r, int c){
    if (!fp) return;
    fprintf(fp, "Move %d — Player %c at (%d,%d)\n", move_no, player, r, c);
}

void logBoard(FILE *fp, char **board, int N){
    if (!fp) return;
    // header
    fprintf(fp, "   ");
    for (int c = 0; c < N; c++) fprintf(fp, "%3d ", c);
    fprintf(fp, "\n   ");
    for (int c = 0; c < N; c++) fprintf(fp, "----");
    fprintf(fp, "-\n");
    // rows
    for (int r = 0; r < N; r++) {
        fprintf(fp, "%2d ", r);
        for (int c = 0; c < N; c++) fprintf(fp, "| %c ", board[r][c]);
        fprintf(fp, "|\n   ");
        for (int c = 0; c < N; c++) fprintf(fp, "----");
        fprintf(fp, "-\n");
    }
    fprintf(fp, "\n");
}



int main(void){
    srand((unsigned)time(NULL));

    int N;
    do {
        printf("Enter the size of the board (3–10): ");
        scanf("%d", &N);
    } while (N < 3 || N > 10);

    printf("\nChoose game mode:\n");
    printf("1. Player vs Player\n");
    printf("2. Player vs Computer\n");
    printf("3. 3 Player\n");
    int mode;
    scanf("%d", &mode);

    char **board = boardInitialize(N);
    boardCreate(N, board, ' ');

    char players[3] = {'X', 'Y', 'Z'};
    int numPlayers = (mode == 3) ? 3 : 2;
    int isComputer[3] = {0, 0, 0};

    if (mode == 2) {
        int choice;
        printf("Do you want to play first? (1=Yes, 2=No): ");
        scanf("%d", &choice);
        if (choice == 1) {
            isComputer[0] = 0; // Player is X
            isComputer[1] = 1; // Computer is O
        } else {
            isComputer[0] = 1; // Computer is X
            isComputer[1] = 0; // Player is O
        }
    }
    FILE *logfp = openLog("game_log.txt", "w");
    if (logfp) {
        fprintf(logfp, "Game start — N=%d, mode=%d, players=", N, mode);
        for (int i = 0; i < numPlayers; i++) fprintf(logfp, "%c%s", players[i], (i+1<numPlayers?",":""));
        fprintf(logfp, "\n\n");
    }

    int turn = 0;
    int moveNum = 1;

    while (1) {
        PrintBoard(N, board);
        printf("Move #%d — Player %c\n", moveNum, players[turn]);
        int r, c;

        if (mode == 2 && isComputer[turn]) {
            if (!ComputerMoves(board, N, &r, &c)) {
                printf("No moves left.\n");
                break;
            }
            printf("Computer chooses: %d %d\n", r, c);
        } else {
            while (1) {
                if (!PlayerMove(&r, &c, players[turn])) {
                    printf("Invalid input. Try again.\n");
                    continue;
                }
                if (r < 0 || r >= N || c < 0 || c >= N) {
                    printf("Out of range. Use 0–%d.\n", N - 1);
                    continue;
                }
                if (!CellEmpty(board, r, c)) {
                    printf("Cell occupied. Try again.\n");
                    continue;
                }
                break;
            }
        }

        board[r][c] = players[turn];
        logMove(logfp, moveNum, players[turn], r, c);
        logBoard(logfp, board, N);
        if (logfp) fflush(logfp);


        if (WinCheck(board, N, players[turn])) {
            PrintBoard(N, board);
            printf("Player %c wins in %d moves!\n", players[turn], moveNum);
            break;
        }
        if (DrawCheck(board, N)) {
            PrintBoard(N, board);
            printf("It's a draw!\n");
            break;
        }

        turn = (turn + 1) % numPlayers;
        moveNum++;
    }
     
    if (logfp) fclose(logfp);
    boardFree(N, board);
    return 0;
}
