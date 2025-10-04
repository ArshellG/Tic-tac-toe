#include <stdio.h>
#include <stdlib.h>
char **boardInitialize(int N){
    char **board = malloc(N * sizeof *board); // creating the rows
    for(int i=0;i<N;i++){
        board[i]=malloc(N *sizeof(char));
    }
    return board;
}
void boardCreate(int N,char **board,char fill){
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){             //creating collumns
            board[i][j]=fill;
        }
    }
}
void boardFree(int N,char **board){
    for (int i = 0; i < N; i++) free(board[i]);
free(board);                                       // freeing the memory
}
void PrintBoard(int N, char **board) {
   
    printf("   ");
    for (int c = 0; c < N; c++) {
        printf("%d   ", c);                             //printing the grid
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

int PlayerMove(int *row,int *col,char player){
    printf("Player %c, enter the row and column(eg;1 2)",player);
    if(scanf("%d %d",row,col)!=2){
        int ch; while ((ch = getchar()) != '\n' && ch != EOF) {}       //getting player input 
        return 0;
    }
    return 1;
}
int CellEmpty(char **board,int r,int c){
    return board[r][c]=' ';                      //checking if the board has empty cells
}
int WinCheck(char **board,int N,char player){
    for(int i=0;i<N;i++){                         // checking for wins
                                     
        int rowWin=1;
        for(int j=0;j<N;j++){
            if(board[i][j]!=player){          //checking for the wins along the rows
                rowWin=0;
                break;
            }
        }
        if(rowWin)return 1;
    }

for (int j = 0; j < N; j++) {
        int colWin = 1;
        for (int i = 0; i < N; i++) {
            if (board[i][j] != player) { colWin = 0; break; }     //checking for wins along the columns
        }
        if (colWin) return 1;
    }
    int mainDiag = 1;
    for (int i = 0; i < N; i++) {
        if (board[i][i] != player) { mainDiag = 0; break; }     //checking along diagonal
    }
    if (mainDiag) return 1;

    
    int antiDiag = 1;
    for (int i = 0; i < N; i++) {
        if (board[i][N - 1 - i] != player) { antiDiag = 0; break; }
    }                                                                //checking for wins along the other diagonal
    if (antiDiag) return 1;

    return 0;
                                          
    
}
int DrawCheck(char **board,int N){
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){              //checking for draws
            if(board[i][j]==' '){
                return 0;
            }
        }
    }
    return 1;
}





int main(void){
    int N;
    do{
    printf("Enter the size of the board(3-10)");   //getting the board size
    scanf("%d",&N);
    }while(N < 3 || N > 10);
    char **board=boardInitialize(N);
    boardCreate(N,board, ' ');            //displaying the baord
    char players[2]={'x','o'};
    int turn=0;
    int movenum =1;
     while(1){
        PrintBoard(N,board);
        printf("Move :%d,player :%c",movenum,players[turn]);          
        int r,c;
        while(1){
            if(!PlayerMove(&r,&c,players[turn])){
                printf("Invalid inputs. Please enter two integers like 1 or 2");
                continue;                                                           //validitating moves
            }

            if (r < 0 || r >= N || c < 0 || c >= N) {
                printf("Out of range. Row and column must be 0..%d\n", N-1);
                continue;
            }
            if (!CellEmpty(board, r, c)) {
                printf("Cell (%d,%d) is occupied. Try another.\n", r, c);
                continue;
            }
            break; // valid & empty
        }


        
         // place the symbol
        board[r][c] = players[turn];

        // check win then draw 
        if (WinCheck(board, N, players[turn])) {
            PrintBoard(N, board);
            printf("Player %c wins in %d moves! \n", players[turn], movenum);
            break;
        }
        if (DrawCheck(board, N)) {
            PrintBoard(N, board);
            printf("It's a draw!\n");
            break;
        }

                                            // next turn
        turn = (turn + 1) % 2;
        movenum++;
    }

    boardFree(N, board);
    return 0;








     }

  
