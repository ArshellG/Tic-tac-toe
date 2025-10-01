#include <stdio.h>
#include <stdlib.h>
char **boardInitialize(int N){
    char **board = malloc(N * sizeof *board); 
    for(int i=0;i<N;i++){
        board[i]=malloc(N *sizeof(char));
    }
    return board;
}
void boardCreate(int N,char **board,char fill){
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            board[i][j]=fill;
        }
    }
}
void boardFree(int N,char **board){
    for (int i = 0; i < N; i++) free(board[i]);
free(board); 
}
void PrintBoard(int N, char **board) {
   
    printf("   ");
    for (int c = 0; c < N; c++) {
        printf("%d   ", c);
    }
    printf("\n");

    
    printf("   ");
    for (int c = 0; c < N; c++) printf("----");
    printf("-\n");

    
    for (int r = 0; r < N; r++) {
        printf("%d  ", r);  
        for (int c = 0; c < N; c++) {
            printf("| %c ", board[r][c]);
        }
        printf("|\n");  

        
        printf("   ");
        for (int c = 0; c < N; c++) printf("----");
        printf("-\n");
    }
}

int main(void){
    int N;
    printf("Enter the size of the board");
    scanf("%d",&N);
    char **board=boardInitialize(N);
    boardCreate(N,board, ' ');
    PrintBoard(N,board);
    boardFree(N,board);
    return 0;

} 