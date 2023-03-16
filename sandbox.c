#include <stdio.h>
#include <stdlib.h>

int main() {
    char* emreler[5] = (char *) calloc(5, sizeof(char*));
    char** emre = (char **) calloc(10, sizeof(char*));
    char* kroston = *emre;
    kroston++;
    char* anne = (*emre)+1;

    printf("%p %p %p %p\n", emre, *emre, kroston, anne);
    printf("%d %d %d %d\n", *emre == NULL, anne == NULL, kroston == NULL, emreler[4] == NULL);
    return 0;
}
