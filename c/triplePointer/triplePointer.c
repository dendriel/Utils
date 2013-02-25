/*
 * Triple pointer and dynamic allocation example.
 * vitor.rozsa
 * vitor.rozsa@hotmail.com
 * 25/02/2013
 */

#include <stdio.h>
#include <stdlib.h>

#define SIZET 100000

static void fill_up(char ***list)
{
    char **ptt = NULL;

    ptt = (char **)malloc(sizeof(char *) * 1);

    // comment the line bellow to make the program fails.
    ptt = (char **)realloc(ptt, sizeof(char *) * SIZET);

    *list = ptt;
}


int main()
{
    int i;
    char **mpt;
    char *pt = "vitor";

    fill_up(&mpt);

    for (i = 0; i < SIZET; i++) {
        printf("%d\n", i);
        mpt[i] = pt;
    }

    free(mpt);

    return 0;
}
