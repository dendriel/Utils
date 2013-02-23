#include <stdio.h>
#include <stdlib.h>


static int server_split_cmd(char *cmd, char ***cmd_args)
{
    char *saveptr;
    char *elem = NULL;
    const char delim[] = " ";
    char *cmd_vet[] = {NULL};
    char *allpt = NULL;
    int ind = 0;

    elem = (char *)strtok_r(cmd, delim, &saveptr);
    if (elem == NULL) {
        return -1;
    }
    printf("\n%s\n", elem);

    printf("\t &cmd_vet[0] = %p\n", &cmd_vet[ind]);
    *cmd_vet = (char *)malloc(sizeof(char));
    if (cmd_vet[ind] == NULL) {
        printf("Failed to allocate memory!\n");
        return -1;
    }
    cmd_vet[ind] = elem;
    ind++;

    printf("\t cmd_vet[0] = %p\n", cmd_vet[ind]);

    elem = (char *)strtok_r(NULL, delim, &saveptr);
    while (elem != NULL) {
        printf("%s\n", elem);

        printf("\t cmd_vet[0] = %p\n", cmd_vet[0]);
        allpt = (char *) realloc(*cmd_vet[0], sizeof(char) * (ind+1));
        if (allpt == NULL) {
            printf("Failed to realloc!\n");
            break;
        }
        cmd_vet[ind] = allpt;
        cmd_vet[ind] = elem;
        elem = (char *)strtok_r(NULL, delim, &saveptr);
    }

/*    int ind = 0;
    char *saveptr = NULL;
    const char *delim = " ";
    char *elem = NULL;
    const char *ctest = 'Z';

    elem = strtok_r(cmd, delim, &saveptr);
    if (elem == NULL) {
        return -1;
    }

    printf("point [0] elem: %s\n", elem);

    *cmd_args = (char *) malloc(sizeof(char));

    

//    printf("point [0] elem: %s\n", elem);
//    cmd_args[ind] = elem;
//    ind++;
//
//    printf("point [1]\n");
//
//    elem = strtok_r(cmd, delim, &saveptr);
//    while (elem != NULL) {
//        cmd_args[ind] = (char *) realloc(cmd_args, sizeof(char));
//        cmd_args[ind] = elem;
//        ind++;
//        printf("point [2]\n");
//    }
//    printf("point [3]\n");
*/
    return (1);
}



int main (void)
{
    char cmd[] = "cmd arg1 arg2 arg3 arg4";
    char **cpt = NULL;
    int n;
    char *value1 = "12345";
    char *value2 = "22222";

    char *mvet[] = {NULL}; // = {"e1", "e2", "e3", "e4"};

    cpt = mvet;

    mvet[0] = (char *) malloc(sizeof(char)*5);
    mvet[0] = value1;
    mvet[1] = value2;

    printf("%s %s\n", cpt[0], cpt[1]);


    n = server_split_cmd(cmd, &cpt);
//
//    if (n == CLI_ERROR) {
//        printf("Failed to split the cmd!\n");
//        return -1;
//    }
//
//    printf("Num of cmds: %d\n", n);
//
//    free(cpt); // ?? maybe
//
    return 0;
}
