/*
 * Will alloc memory dinamically while there is 
 * 'args' to fill the array.
 * vitor.rozsa
 * vitor.rozsa@hotmail.com
 * 25/02/2013
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static int server_split_cmd(char *cmd, char ***cmd_args)
{
    char *saveptr;
    char *elem = NULL;
    char **allpt = NULL;
    char **cmd_vet;
    const char *delim = " ";
    int ind = 0;

    elem = strtok_r(cmd, delim, &saveptr);
    if (elem == NULL) {
        return -1;
    }

    cmd_vet = (char **)malloc(sizeof(char *));
    if (cmd_vet == NULL) {
        printf("Failed to allocate memory!\n");
        return -1;
    }
    cmd_vet[ind] = elem;
    ind++;

    elem = strtok_r(NULL, delim, &saveptr);
    while (elem != NULL) {

        allpt = (char **) realloc(cmd_vet, sizeof(char *) * (ind+1));
        if (allpt == NULL) {
            printf("Failed to realloc!\n");
            break;
        }
        cmd_vet = allpt;
        cmd_vet[ind] = elem;
        elem = strtok_r(NULL, delim, &saveptr);
        ind++;
    }

    allpt = (char **) realloc(cmd_vet, sizeof(char *) * (ind+1));
    if (allpt == NULL) {
        printf("Failed to realloc!\n");
        return -1;
    }
    cmd_vet = allpt;
    cmd_vet[ind] = NULL;

    *cmd_args = cmd_vet;

    return 0;
}



int main (void)
{
    char cmd[] = "cmd arg1 arg2 arg3 arg4";
    char **cpt = NULL;
    int n;

    n = server_split_cmd(cmd, &cpt);

    if (n != 0) {
        printf("Failed to split the cmd!\n");
        return -1;
    }

    n = 0;
    while(cpt[n] != NULL) {

        printf("%d - %s\n", n, cpt[n]);
        n++;
    }


    free(cpt);
    return 0;
}
