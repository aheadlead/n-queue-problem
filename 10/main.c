#include <stdio.h>
#include <pthread.h>
#define MAXN 32
#define MAX_THREAD_NUMBER 1
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
pthread_attr_t attr;

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
    int id=(long long)_id;
    size_t i;

    /* TODO 这里有一个奇怪的错误。
     *
     * 编译器: Apple LLVM version 6.0 (clang-600.0.54) (based on LLVM 3.5svn) 
     * 系统: OS X 10.10 Yosemite
     * 遇到问题时的编译参数: gcc main.c -O3 -g
     *
     * 当我引入了pthread多线程库，使得这个算法可以多线程运行后。
     * 如果不加上下面这一行fprintf(stderr, " ");，
     * 系统会提示"Bus error: 10"。
     * 
     * 事实上直接printf(" "); 却是能正常工作的（奇怪吧），
     * 但是这样就会有一个空格输出到stdout，
     * 而我的测试脚本（根目录下的run.py）依赖于stdout的结果。
     * 
     * 下面这行语句如果改为"printf("");"的话，问题依旧。
     * 而且这行语句必须在这一行和这一行之前执行，否则会出现问题。
     *
     * 使用lldb对程序进行调试，因为开启了-O3参数，调试很蛋疼。
     * （如果您有开了优化参数的调试经验，请不吝赐教。）
     *
     * aheadlead 2014年11月13日 星期四 17时10分20秒 CST
     *
     */
    fprintf(stderr, " ");

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

    pthread_attr_init(&attr);
    pthread_attr_setstacksize(&attr, 1*1048576);
    for (i=0; i<MAX_THREAD_NUMBER; ++i)
        pthread_create(&thread_pool[i], &attr, multithreading, (void*)(long long)i);

    for (i=0; i<MAX_THREAD_NUMBER; ++i)
        pthread_join(thread_pool[i], NULL);

    for (int i=0; i<N/2; ++i)
        solution_total = solution_total + 2*task_queue[i].ans;
    if (N & 1)
        solution_total = solution_total + task_queue[task_queue_length-1].ans;
    
    printf("%d\n", solution_total);
    return 0;
}
