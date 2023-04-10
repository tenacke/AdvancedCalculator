#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "functions.h"

// hash function
int hash(char* str){
    int hash = 0;
    for (int i = 0; i < strlen(str); i++){
        hash += str[i];
    }
    return hash % 256;
}

// check if the variable is defined and return its value
// if not return NULL
char* getVariable(Variable* table, char* key){
    int index = hash(key);
    while ((table+index)->key != NULL && !compare((table+index)->key, key)){
        index++;
    }
    if ((table+index)->key == NULL) {
        return "0";
    }
    return (table+index)->value;
}

// set the variable to the table
void setVariable(Variable* table, char* key, char* value){
    int index = hash(key);
    while ((table+index)->key != NULL && !compare((table+index)->key, key)){
        index++;
    }
    if ((table+index)->key == NULL) {
        (table+index)->key = key;
    }
    (table+index)->value = value;
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
IntStack* initializeIntStack(){
    return (IntStack*) malloc(sizeof(IntStack));
}

// push the char or int to the stacks
void push(Stack* stack, char str){
    (*stack).elems[(*stack).size++] = str;
}
void pushInt(IntStack* stack, lli x){
    (*stack).elements[(*stack).size++] = x;
}

// pop the char or int from the stacks
char pop(Stack* stack){
    return (*stack).elems[--(*stack).size];
}
lli popInt(IntStack* stack){
    return (*stack).elements[--(*stack).size];
}

// peek the char at the top of the stack
char peek(Stack* stack){
    return (*stack).elems[(*stack).size-1];
}

// get the sizes of the stacks
int getSize(Stack* stack){
    return (*stack).size;
}
int getIntSize(IntStack* stack){
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
lli performOp(lli a, lli b, char c) {
    if (c == '+') {
        return a+b;
    }
    else if (c == '-') {
        return a-b;
    }
    else if (c == '*') {
        return a*b;
    }
    else if (c == '&') {
        return a&b;
    }
    else if (c == '|') {
        return a|b;
    }
    else if (c == '^') {
        return a^b;
    }
    else if (c == '!') {
        return a>>b;
    }
    else if (c == '$') {
        return a<<b;
    }
    else if (c == '@') {
        return (a>>b)|(a<<(64-b));
    }
    else if (c == '#') {
        return (a<<b)|(a>>(64-b));
    }
    else if (c == '%') {
        return a%b;
    }
    else if (c == '/'){
        return a/b;
    }

    return 0;
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

