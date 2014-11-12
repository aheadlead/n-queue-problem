#include <stdio.h>
#include <pthread.h>
#define MAXN 32
#define MAX_THREAD_NUMBER 2
int N;
int solution_total={0};
int upperlim;

struct task
{
    struct Para
    {
        int row, ld, rd;
    }para;
    int ans;
};

struct task task_queue[MAXN]={{{0, 0, 0}, 0}};
size_t task_queue_length=0;
pthread_t thread_pool[MAX_THREAD_NUMBER];

void test_non_recursive(int _row, int _ld, int _rd, int id)
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
                task_queue[id].ans++;
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

void* multithreading(void* _id)
{
    long long id=(long long)_id;
    size_t i;

    for (i=id; i<task_queue_length; i=i+MAX_THREAD_NUMBER)
    {
        test_non_recursive(task_queue[i].para.row,
                task_queue[i].para.ld,
                task_queue[i].para.rd,
                i);
    }

    return NULL; 
}

int main()
{
    int i;
    FILE* in=fopen("in", "r");
    fscanf(in, "%d", &N);
    fclose(in);

    upperlim = (1<<N)-1;

    for (i=0; i<N/2; ++i)
        task_queue[task_queue_length++] = (struct task){{1<<i, 1<<(i+1), (i-1>=0)?(1<<(i-1)):0}, 0};
    
    if (N & 1)
        task_queue[task_queue_length++] = (struct task){{1<<(N/2), 1<<(N/2+1), 1<<(N/2-1)}, 0};

    for (i=0; i<MAX_THREAD_NUMBER; ++i)
        pthread_create(&thread_pool[i], NULL, multithreading, (void*)(long long)i);

    for (i=0; i<MAX_THREAD_NUMBER; ++i)
        pthread_join(thread_pool[i], NULL);

    for (int i=0; i<N/2; ++i)
        solution_total = solution_total + 2*task_queue[i].ans;
    if (N & 1)
        solution_total = solution_total + task_queue[task_queue_length-1].ans;
    
    printf("%d\n", solution_total);
    return 0;
}
