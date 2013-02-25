#include <stdio.h>


int main(void)
{
    int a = 10;
    int *pt = NULL;
    int **ptt = NULL;
    int ***pttt = NULL;

    pt = &a;
    ptt = &pt;
    pttt = &ptt;

    printf("\ta  = %d\n"
           "\t&a = %p\n\n",
           a, &a
        );

    printf("\t*pt = %d\n"
           "\tpt  = %p\n"
           "\t&pt = %p\n\n",
            *pt, pt, &pt
        );

    printf("\t**ptt = %d\n"
           "\t*ptt  = %p\n"
           "\tptt   = %p\n"
           "\t&ptt  = %p\n\n",
            **ptt, *ptt, ptt, &ptt
        );

    printf("\t***pttt = %d\n"
           "\t**pttt  = %p\n"
           "\t*pttt   = %p\n"
           "\tpttt    = %p\n"
           "\t&pttt   = %p\n\n",
            ***pttt, **pttt, *pttt, pttt, &pttt
        );

    return 0;
}
