#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "functions.h"

extern FILE* output;
// hash function
int hash(char* str){
    int hash = 0;
    for (int i = 0; i < strlen(str); i++){
        if (str[i] != '%')
            hash += str[i];
    }
    return hash % 256;
}

// check if the variable is defined and return its value
// if not return NULL
int getVariable(Variable* table, char* key){
    char* ters = (char *) malloc(sizeof(key)+1);
    char* temp = ters;
    *temp++ = '%';
    for (int i = strlen(key)-1; i >= 0 ; i--) {
        *temp++ = *(key+i);
    }
    int index = hash(ters);
    while ((table+index)->key != NULL && !compare((table+index)->key, ters)){
        index++;
    }
    free(ters);
    if ((table+index)->key == NULL) {
        return 0;
    }
    return 1;
//    return (table+index)->value;
}

// set the variable to the table
void setVariable(Variable* table, char* key, char* value){
    // TODO should check if it exists, if not call ALLOCA, in both cases call STORE
    int index = hash(key);
    while ((table+index)->key != NULL && !compare((table+index)->key, key)){
        index++;
    }
    if ((table+index)->key == NULL) {
        fprintf(output, ALLOCA, key);
        (table+index)->key = key;
    }
    fprintf(output, STORE, value, key);
}

// returns the function symbol if the string is a function
// otherwise returns NULL
char* isFunction(char* str){
    if (strcmp(str, "rox") == 0) {
        return "^";
    }else if (strcmp(str, "sr") == 0) {
        return "!";
    }else if (strcmp(str, "sl") == 0) {
        return "$";
    }else if (strcmp(str, "rr") == 0) {
        return "@";
    }else if (strcmp(str, "rl") == 0) {
        return "#";
    }else if (strcmp(str, "ton") == 0) {
        return "~";
    }
    return NULL;
}

// returns 1 if the string is an operator
int isOperator(char str) {
    return (str) == '+' || (str) == '-' || (str) == '*' || (str) == '&' || (str) == '|' || (str) == '/' || (str) == '%';
}

// strip the left side of the string
char* leftStripper(char* str) {
    while (isspace(*str) || *str == '\n') {
        str++;
    }
    return str;
}

// strip the right side of the string
char* rightStripper(char* str) {
    char* end = str + strlen(str) - 1;
    while (end > str && (*end == ' ' || *end == '\n')) {
        *end-- = '\0';
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

// initialize the stacks
Stack* initializeStack(){
    return (Stack*) malloc(sizeof(Stack));
}
// push the char or int to the stacks
void push(Stack* stack, char str){
    (*stack).elems[(*stack).size++] = str;
}

// pop the char or int from the stacks
char pop(Stack* stack){
    return (*stack).elems[--(*stack).size];
}

// peek the char at the top of the stack
char peek(Stack* stack){
    return (*stack).elems[(*stack).size-1];
}

// get the sizes of the stacks
int getSize(Stack* stack){
    return (*stack).size;
}

PStack* initializePStack(){
    return (PStack*) malloc(sizeof(PStack));
}
// push the char or int to the stacks
void pushP(PStack* stack, char* str){
    (*stack).elems[(*stack).size++] = str;
}

// pop the char or int from the stacks
char* popP(PStack* stack){
    return (*stack).elems[--(*stack).size];
}

// peek the char at the top of the stack
char* peekP(PStack* stack){
    return (*stack).elems[(*stack).size-1];
}

// get the sizes of the stacks
int getSizeP(PStack* stack){
    return (*stack).size;
}

//return our assigned precedence values
int getPrecedence(char* str){
    switch (*str){
        case '^':
        case '!':
        case '$':
        case '@':
        case '#':
            return 1;
        case '|':
            return 2;
        case '&':
            return 3;
        case '+':
            return 4;
        case '-':
            return 5;
        case '*':
        case '/':
        case '%':
            return 6;
        case '~':
            return 7;
    } 
    return 0;
}

//take 2 integers and an operator and perform the appropriate operation
char* performOp(char* a, char* b, char c) {
    char* ret = getNewRegister();
    if (c == '+') {
        fprintf(output, ADD, ret, a, b);
    }
    else if (c == '-') {
        fprintf(output, SUB, ret, a, b);
    }
    else if (c == '*') {
        fprintf(output, MUL, ret, a, b);
    }
    else if (c == '&') {
        fprintf(output, AND, ret, a, b);
    }
    else if (c == '|') {
        fprintf(output, OR, ret, a, b);
    }
    else if (c == '^') {
        fprintf(output, XOR, ret, a, b);
    }
    else if (c == '!') {
        fprintf(output, RSHIFT, ret, a, b);
    }
    else if (c == '$') {
        fprintf(output, LSHIFT, ret, a, b);
    }
    else if (c == '%') {
        fprintf(output, MOD, ret, a, b);
    }
    else if (c == '/'){
        fprintf(output, DIV, ret, a, b);
    }
    return ret;
}

// compares two strings
int compare(char* str1, char* str2){
    int i = 0;
    while (str1[i] != '\0' && str2[i] != '\0'){
        if (str1[i] != str2[i]){
            return 0;
        }
        i++;
    }
    if (str1[i] == '\0' && str2[i] == '\0'){
        return 1;
    }
    return 0;
}

char* getNewRegister(){
    static int regNum;
    char *reg;
    asprintf(&reg, "%%ekt%d", ++regNum);
    return reg;
}

