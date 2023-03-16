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
    while ((table+index)->key != NULL && (table+index)->key != key){
        index++;
    }
    if ((table+index)->key == key) {
        return (table+index)->value;
    }
    return "0";
}

// set the variable to the table
void setVariable(Variable* table, char* key, char* value){
    int index = hash(key);
    while ((table+index)->key != NULL && (table+index)->key != key){
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
    if (strcmp(str, "rox ") == 0) {
        return "^";
    }else if (strcmp(str, "sr ") == 0) {
        return "!";
    }else if (strcmp(str, "sl ") == 0) {
        return "$";
    }else if (strcmp(str, "rr ") == 0) {
        return "@";
    }else if (strcmp(str, "rl ") == 0) {
        return "#";
    }else if (strcmp(str, "ton ") == 0) {
        return "~";
    }
    return NULL;
}

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

// peek the char at the top of the stack
char peek(Stack* stack){
    return (*stack).elems[(*stack).size-1];
}

// get the size of the stack
int getSize(Stack* stack){
    return (*stack).size;
}

int getPrecedence(char* str){
    switch (*str){
        case '^':
        case '!':
        case '$':
        case '@':
        case '#':
            return 1;
        case '+':
            return 2;
        case '-':
            return 3;
        case '*':
            return 4;
        case '&':
            return 5;
        case '|':
            return 6;
        case '~':
            return 7;
    } 
    return 0;
}