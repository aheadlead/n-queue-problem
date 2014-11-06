#include <stdio.h>
#define MAXN 32
#define false 0
#define true 1

#define ABS(x) ((x)>0?(x):-(x))

int chessboard[MAXN]={0};
int N;
int solution_total = 0;

// 判断当前局面是否合法
int is_valid()
{
    int queen1, queen2;

    for (queen1=0; queen1<N; ++queen1)
    {
        for (queen2=0; queen2<queen1; ++queen2)
        {
            if (chessboard[queen1] == chessboard[queen2])
                return false;
            if (ABS(chessboard[queen1]-chessboard[queen2]) == ABS(queen1-queen2))
                return false;
        }
    }

    return true;
}

// 把第queen_number号皇后摆到棋盘上去
void place_queen(int queen_number)
{
    int row;
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
        // 将第queen_number号皇后摆到棋盘的row行column列
        chessboard[queen_number] = row;
        // 放置下一个皇后
        place_queen(queen_number+1);
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
