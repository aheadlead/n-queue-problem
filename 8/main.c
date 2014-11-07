#include <stdio.h>
#define MAXN 32
#define false 0
#define true 1
int N;
int solution_total = 0;
int upperlim;
void test(int row, int ld, int rd)
{
    int pos, p;
    if (row != upperlim)
    {
        pos = upperlim & (~(row | ld | rd));
        while (pos)
        {
            p = pos & (~pos + 1);
            pos -= p;
            test(row | p, (ld | p) << 1, (rd | p) >> 1);
        }
    }
    else
        solution_total++;
}

int main()
{
    int i;
    FILE* in=fopen("in", "r");
    fscanf(in, "%d", &N);
    fclose(in);

    upperlim = (1<<N)-1;

    for (i=0; i<N/2; ++i)
        test(1<<i, 1<<(i+1), 1<<(i-1));
    solution_total = solution_total*2;
    
    if (N & 1)
        test(1<<(N/2), 1<<(N/2+1), 1<<(N/2-1));

    printf("%d\n", solution_total);
    return 0;
}
