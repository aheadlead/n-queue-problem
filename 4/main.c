#include <stdio.h>
#define MAXN 32
#define false 0
#define true 1

#define ABS(x) ((x)>0?(x):-(x)) // ABS(x) 返回x的绝对值

int chessboard[MAXN]={0};
int N;
int solution_total = 0;
int row_available[MAXN]={0};

int is_vaild(int current_queen)
{
    int queen;
    for (queen=0; queen<current_queen; ++queen)
        if (chessboard[queen] == chessboard[current_queen] || 
                ABS(chessboard[queen]-chessboard[current_queen]) == ABS(queen-current_queen))
            return false;
    return true;
}

// 把第queen_number号皇后摆到棋盘上去
void place_queen(int queen_number)
{
    int row;
    if (queen_number == N)
    {
        // 如果已经摆了N个皇后
        solution_total = solution_total+1;
        return;
    }
    // 枚举新皇后的位置
    for (row=0; row<N; ++row)
    {
        // 将第queen_number号皇后摆到棋盘的row行queen_number列
        if (row_available[row] == 0)
        {
            row_available[row] = 1;
            chessboard[queen_number] = row;
            if (is_vaild(queen_number)) // 判断当前局面下queen_number号皇后与之前的换后是否有冲突
            {
                place_queen(queen_number+1); // 放置下一个皇后
            }
            row_available[row] = 0;
        }
    }
}

int main()
{
    FILE* in=fopen("in", "r");
    fscanf(in, "%d", &N);
    fclose(in);

    // 开始放置皇后
    place_queen(0);
    
    printf("%d\n", solution_total);
    return 0;
}
