#include <stdio.h>
#define MAXN 32
#define false 0
#define true 1

int chessboard[MAXN][MAXN]={{0}};
int N;
int solution_total = 0;

// 判断当前局面是否合法
int is_valid()
{
    int row, column;
    int step;

    for (row=0; row<N; ++row)
    {
        for (column=0; column<N; ++column)
        {
            if (chessboard[row][column] == 1)
            {
                // 遍历每个皇后
                for (step=1; row+step<N; ++step)    // 确定右方没有皇后
                    if (chessboard[row+step][column] == 1)
                        return false;
                for (step=1; column+step<N; ++step) // 确定下方没有皇后
                    if (chessboard[row][column+step] == 1)
                        return false;
                for (step=1; row+step<N && column+step<N; ++step)   // 确定右下方没皇后
                    if (chessboard[row+step][column+step] == 1)
                        return false;
                for (step=1; row+step<N && column-step>=0; ++step)  // 确定左下方没皇后
                    if (chessboard[row+step][column-step] == 1)
                        return false;
            } 
        }
    }
    return true;
}

// 把第queen_number号皇后摆到棋盘上去
void place_queen(int queen_number)
{
    int row, column;
    if (queen_number == N)
    {
        // 如果已经摆了N个皇后
        // 就判断当前局面是否合法
        if (is_valid())
            solution_total = solution_total+1;
        return;
    }
    // 枚举新皇后的位置
    for (row=0; row<N; ++row)
    {
        for (column=0; column<N; ++column)
        {
            if (chessboard[row][column] == 0)
            {
                // 将第queen_number号皇后摆到棋盘的row行column列
                chessboard[row][column] = 1;
                // 放置下一个皇后
                place_queen(queen_number+1);
                // 将第queen_number号皇后从棋盘的row行column列上拿下来
                chessboard[row][column] = 0;
            }
        }
    }
}

// 返回a的阶乘
int factorial(int a)
{
    int ret=1;
    for (; a>0; a--)
        ret = ret*a;
    return ret;
}

int main()
{
    FILE* in=fopen("in", "r");
    fscanf(in, "%d", &N);
    fclose(in);

    // 开始放置皇后
    place_queen(0);
    
    // 去除重复的解
    solution_total = solution_total/(factorial(N));

    printf("%d\n", solution_total);
    return 0;
}
