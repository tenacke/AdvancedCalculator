#include <stdio.h>
#include <stdlib.h>

int main() {
    char** emre = (char **) calloc(10, sizeof(char*));
    char* anne = (*emre)+1;

    printf("%d\n", (int*) anne);
    return 0;
}
