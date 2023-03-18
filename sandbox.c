#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "functions.h"

int main() {
    Variable* variables = (Variable*) calloc(256, sizeof(Variable));
    char* str = malloc(4);
    strcpy(str, "123");
    char* str2 = "123";

    setVariable(variables, "a", "10");
    setVariable(variables, "b", "20");
    setVariable(variables, "c", "30");
    setVariable(variables, "A ", "11");
    setVariable(variables, "B ", "21");
    setVariable(variables, "C ", "31");

    printf("%s\n", getVariable(variables, "a"));
    printf("%s\n", getVariable(variables, "b"));
    printf("%s\n", getVariable(variables, "c"));
    printf("%s\n", getVariable(variables, "A "));
    printf("%s\n", getVariable(variables, "B "));
    printf("%s\n", getVariable(variables, "C "));

    printf("%d", str == str2);

    return 0;
}
