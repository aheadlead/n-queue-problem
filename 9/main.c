#include <stdio.h>
#define MAXN 32
int N;
int solution_total={0};
int upperlim;

void test_non_recursive(int _row, int _ld, int _rd)
{
    int row[MAXN];
    int* prow=row-1;
    int ld[MAXN];
    int* pld=ld-1;
    int rd[MAXN];
    int* prd=rd-1;
    int pos[MAXN];
    int* ppos=pos-1;
    int* ppos_m1=pos-1;
    int p;
    *(++prow) = _row;
    *(++pld) = _ld;
    *(++prd) = _rd;
    do
    {
        *(++ppos) = upperlim & (~((*prow)|(*pld)|(*prd)));
        if (0 == *ppos)
        {
            if ((*prow) == upperlim)
                solution_total++;
            while (0 == *ppos)
            {
                ppos--;
                prow--;
                pld--;
                prd--;
            }
        }
        p = (*ppos) & (~(*ppos) + 1);
        *ppos -= p;
        *(prow+1) = (*prow) | p; prow++;
        *(pld+1) = ((*pld) | p) << 1; pld++;
        *(prd+1) = ((*prd) | p) >> 1; prd++;
    }
    while (ppos != ppos_m1);
}

int main()
{
    int i;
    FILE* in=fopen("in", "r");
    fscanf(in, "%d", &N);
    fclose(in);

    upperlim = (1<<N)-1;

    for (i=0; i<N/2; ++i)
        test_non_recursive(1<<i, 1<<(i+1), (i-1>=0)?(1<<(i-1)):0);
    solution_total = solution_total*2;
    
    if (N & 1)
        test_non_recursive(1<<(N/2), 1<<(N/2+1), 1<<(N/2-1));

    printf("%d\n", solution_total);
    return 0;
}
