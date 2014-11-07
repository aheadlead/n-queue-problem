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
    FILE* in=fopen("in", "r");
    fscanf(in, "%d", &N);
    fclose(in);

    upperlim = (1<<N)-1;
    test(0, 0, 0);

    printf("%d\n", solution_total);
    return 0;
}
