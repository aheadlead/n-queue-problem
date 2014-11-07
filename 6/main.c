#include <stdio.h>
#define MAXN 32
#define false 0
#define true 1

#define ABS(x) ((x)>0?(x):-(x)) // ABS(x) 返回x的绝对值

int chessboard[MAXN]={0};
int N;
int solution_total = 0;
int row_available[MAXN]={0};
int rdiag_available[MAXN+MAXN-1]={0}, ldiag_available[MAXN+MAXN-1]={0};

// 把第queen_number号皇后摆到棋盘上去
//   皇后从start_row行开始放，直到end_row行（前闭后开）
void place_queen(int queen_number, int start_row, int end_row)
{
    int row;
    if (queen_number == N)
    {
        // 如果已经摆了N个皇后
        solution_total = solution_total+1;
        return;
    }
    // 枚举新皇后的位置
    for (row=start_row; row<end_row; ++row)
    {
        if (row_available[row] == 0 && 
                rdiag_available[row-queen_number+N-1] == 0 &&   // 右下对角线方向 
                ldiag_available[row+queen_number] == 0) // 左下对角线方向
        {
            row_available[row] = 1;
            rdiag_available[row-queen_number+N-1] = 1;
            ldiag_available[row+queen_number] = 1;
            chessboard[queen_number] = row; // 将第queen_number号皇后摆到棋盘的row行queen_number列
            
            place_queen(queen_number+1, 0, N); // 放置下一个皇后
            
            row_available[row] = 0;
            rdiag_available[row-queen_number+N-1] = 0;
            ldiag_available[row+queen_number] = 0;
        }
    }
}

int main()
{
    FILE* in=fopen("in", "r");
    fscanf(in, "%d", &N);
    fclose(in);

    // 开始放置皇后
    place_queen(0, 0, N/2);

    // 由于上下镜像导致对称的关系，解的数量能直接乘以二
    solution_total = solution_total*2;

    // 当棋盘边长为奇数的时候要特别考虑一下第一个皇后放N/2行的情况（不要忘了是从第0行开始数）
    if (N % 2 == 1)
        place_queen(0, N/2, N/2+1);
    
    printf("%d\n", solution_total);
    return 0;
}
