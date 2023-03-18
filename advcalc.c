#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "functions.h"

// functions
char* XOR = "xor"; // ^
char* RS = "rs"; // !
char* LS = "ls"; // $
char* RR = "rr"; // @
char* LR = "lr"; // #
char* NOT = "not"; // ~

// variables
Variable* variables;

enum tokens {
    NUMBER,
    VARIABLE,
    OPERATOR,
    FUNCTION,
    LEFT_PARENTHESIS,
    RIGHT_PARENTHESIS,
    NONE
};

// evaluate the expression and return NULL if it is not valid
int* evaluateExpression(char* str);

// convert infix expression to postfix expression
char* infixToPostfix(char* str, Variable* variables);

int main(){
    char str[256+1] = "";
    printf("> ");
    variables = (Variable*) calloc(256, sizeof(Variable));

    while (fgets(str, sizeof(str), stdin)) {

        // check for comments
        split(str, '%');
        // check '=' for the line is assignment or not
        char* right = split(str, '=');
        
        if (right){ // means that line is assignment
                char *left, temp; //left hand side
                left = strip(str);
                int len = strlen(left) -1;
                // check if the left side is valid
                for (int i = 0; i < len; i++) {
                    temp = *(left+i);
                    *(left+i) = *(left+len);
                    *(left+len) = temp;
                    len--;
                }

                char* func = isFunction(left);
                int invalid = 0;
                for (int i = 0; i < strlen(left); i++) {
                    if (!isalpha(*(left+i))) {
                        invalid = 1;
                        break;
                    }
                }

                if (func || invalid) {
                    printf("line 65");
                    printf("Error!");
                    printf("\n> ");
                    continue;
                }
                

            //evaluate the expression
            right = strip(right); //right hand side
            right = infixToPostfix(right, variables); //convert to postfix
            int* result = evaluateExpression(right);
            if (result) {
                int a = (int)result;
                char* res = (char*) malloc(sizeof(char)*25);
                char* res1 = res;
                bool isNeg = false;
                if (a<0) {
                    isNeg = true;
                    a = a*-1;
                }

                while (a>0) {
                    *res++ = '0'+a%10;
                    a= a/10;
                }
                if (isNeg) {
                    *res++='-';
                }
                *res = '\0';
                setVariable(variables, left, res1);

            } else{
                printf("Error!");
            }

        }else{ // means that line is not assignment
            // evaluate the expression
            char* expr = strip(str);
            expr = infixToPostfix(expr, variables);
            int* result = evaluateExpression(expr);
            if (result){
            printf("%d ", result);
            }else{
            printf("Error!");
            }
        }
        printf("\n");
        printf("> ");
    }
    return 0;
}

char* infixToPostfix(char* str, Variable* variables){
    enum tokens lastToken = OPERATOR;
    Stack* operations = initializeStack();
    // memory for digits and letters
    Stack* memory = initializeStack(); 
    Stack* functions = initializeStack(); 

    // result string in postfix form
    char* result = (char*) malloc(sizeof(char)*512);
    char* copy = result;
    for (; (*str) != '\0'; str++) {
        if (isdigit(*str)) { 
            // add the digit to the memory
            push(memory, *str);
            char next = *(str+1);
            if (isspace(next) || next == ')' || isOperator(next) || next == ','){
                // add the memory to the result and clear the memory
                // it is reversed but it is a feature not a bug :)
                while (getSize(memory) > 0) {
                    *result++ = pop(memory);
                }
                *result++ = ' ';
                lastToken = NUMBER;
            } else if (!isdigit(next) || lastToken == NUMBER || lastToken == VARIABLE || lastToken == RIGHT_PARENTHESIS || lastToken == FUNCTION){ 
                // if the next character is not digit or space or ')' or operator or digit that means it is error
                printf("line 102");
                return NULL;
            }
        } else if (isalpha(*str)) {
            push(memory, *str);
            char next = *(str+1);
            if (isspace(next) || next == '(' || next == ')' || isOperator(next) || next == ','){
                // add the memory to the result and clear the memory
                char* temp = (char*) malloc(sizeof(char)*getSize(memory)); 
                char* copy = temp;
                while (getSize(memory) > 0) {
                    *temp++ = pop(memory);
                }
                *temp = '\0';
                char* func = isFunction(copy);
                if (func) {
                    if (*func == '~'){
                        push(operations, *func);
                    }else{
                        push(functions, *func);
                    }
                    lastToken = FUNCTION;
                } else {
                    char* var = getVariable(variables, copy);
                    while (*var != '\0') {
                        *result++ = *var++;
                    }
                    *result++ = ' ';
                    lastToken = VARIABLE;
                }

            } else if (!isalpha(next) || lastToken == NUMBER || lastToken == VARIABLE || lastToken == RIGHT_PARENTHESIS || lastToken == FUNCTION){ 
                // if the next character is not letter or space or parenthesis or operator or digit that means it is error
                printf("line 136");
                return NULL;
            }
        } else if (isspace(*str)) {
            // ignore the spaces
        } else if (*str == '(') {
            // push the '(' to the stack
            push(operations, *str);
            char* next = str+1;
            while (*next == ' '){
                next++;
            }
            if (isOperator(*next) || *next == ')' || lastToken == NUMBER || lastToken == VARIABLE || lastToken == RIGHT_PARENTHESIS) {
                printf("line 149");
                return NULL;
            }
            lastToken = LEFT_PARENTHESIS;
        } else if (*str == ')') {
            if (lastToken == LEFT_PARENTHESIS || lastToken == OPERATOR || lastToken == FUNCTION){
                printf("line 155");
                return NULL;
            }
            // pop the stack until '('
            // add the popped operators to the result
            char operation = ')';
            while (operation != '(') {  //(((((emre + 1234523523)))))
                if (getSize(operations) == 0){
                    printf("line 163");
                    return NULL; // if there is no ( that means it it error
                }
                operation = pop(operations);
                if (operation!='(') {
                    *result++ = operation;
                    *result++ = ' ';
                }
            }

            lastToken = RIGHT_PARENTHESIS;
        } else if (isOperator(*str)) {
            if (lastToken == LEFT_PARENTHESIS || lastToken == OPERATOR || lastToken == FUNCTION){
                printf("line 176");
                return NULL;
            }
            // pop the stack until the precedence of the current operator is higher than the top of the stack
            int currPrec = getPrecedence(str);
            if (getSize(operations)>0) {
                char c = peek(operations);

                while (getSize(operations)>0 && currPrec<=getPrecedence(&c)) {
                    *result++ = pop(operations);
                    *result++ = ' ';
                    c = peek(operations);
                }
            }
            push(operations, *str);

            lastToken = OPERATOR;
            
        } else if (*str == ','){
            if (getSize(functions)==0 || lastToken == LEFT_PARENTHESIS || lastToken == OPERATOR || lastToken == FUNCTION){
                printf("line 196");
                return NULL;
            }
            // use the comma as an operator for a function
            char op = pop(functions);

            int currPrec = getPrecedence(&op);
            if (getSize(operations)>0) {
                char c = peek(operations);

                while (getSize(operations)>0 && currPrec<=getPrecedence(&c)) {
                    *result++ = pop(operations);
                    *result++ = ' ';
                    c = peek(operations);
                }
            }
            push(operations, op);
            lastToken = OPERATOR;
        } else {
            // if the character is not valid that means it is error
            return NULL;
        }
    }

    // empty the operations stack
    while (getSize(operations)) {
        char operation = pop(operations);
        if (operation == '(' || operation == ')')
            return NULL;
        *result++ = operation;
        *result++ = ' ';
    }
    if (lastToken == OPERATOR || lastToken == LEFT_PARENTHESIS || lastToken == FUNCTION)
        return NULL;

    *result = '\0';
    return copy;
}

int* evaluateExpression(char* str){
    if (str) {
        int power = 1;
        int myVar = -1;
        IntStack* res = initializeIntStack();

        while (*str!='\0') {
            if (isdigit(*str)) {
                if (myVar == -1) {
                    myVar = 0;
                }
                myVar+=((int)(*str)-(int)('0'))*power;
                power*=10;
                if (*(str+1)=='-') {
                    myVar = myVar*-1;
                    str++;
                }
            }
            else if (*str == ' ' && myVar!=-1) {
                pushInt(res, myVar);
                myVar = -1;
                power = 1;
            }
            else if (*str == '~'){
                pushInt(res, ~popInt(res));

            }
            else if (*str != ' '){
                pushInt(res, performOp(popInt(res), popInt(res), *str));
            }
            str++;
        }
        int* a = popInt(res);
        return a;
        }

    return NULL;
}