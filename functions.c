#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "functions.h"

// returns 1 if the string is an operator
int isOperator(char str) {
    return (str) == '+' || (str) == '-' || (str) == '*' || (str) == '&' || (str) == '|';
}

// strip the left side of the string
char* leftStripper(char* str) {
    while (isspace(*str)) {
        str++;
    }
    return str;
}

// strip the right side of the string
char* rightStripper(char* str) {
    char* end = strrchr(str, ' ');
    while (end && *(end+1) == '\0') {
        *end = '\0';
        end = strrchr(str, ' ');
    }
    return str;
}

// strip the string
char* strip(char* str) {
    return leftStripper(rightStripper(str));
}

// split the string into two parts by the first character
char* split(char* str, char find) {
    char* end = strchr(str, find);
    if (end){
        *end = '\0';
        return end+1;
    } else{
        return NULL;
    }
}

// initialize the stack
Stack* initializeStack(){
    return (Stack*) malloc(sizeof(Stack));
}

// push the char to the stack
void push(Stack* stack, char str){
    (*stack).elems[(*stack).size++] = str;
}

// pop the char from the stack
char pop(Stack* stack){
    return (*stack).elems[--(*stack).size];
}

// get the size of the stack
int getSize(Stack* stack){
    return (*stack).size;
}

int getPrecedence(char* str){
    switch (*str){
        // TODO make precedence logic
    }
    return 0;
}
