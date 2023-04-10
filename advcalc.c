#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "functions.h"

// variables
Variable* variables;
enum tokens lastToken;

// evaluate the expression and return NULL if it is not valid
lli evaluateExpression(char* str);

// convert infix expression to postfix expression
char* infixToPostfix(char* str);

//main entrance of the code
int main(int argv, char *args[]){
    char *FILENAME = args[argv-1];
    size_t length = strlen(FILENAME);
    char* outputFileName;
    strncpy(outputFileName, FILENAME, length-4);
    strcpy(outputFileName+length-4, ".ll");
    int lineNumber = 0;
    char str[256+2] = "";

    variables = (Variable*) calloc(256, sizeof(Variable)); //variable hashmap initialization
    input = fopen(FILENAME, "r");
    output = fopen(outputFileName, "w");

    fprintf(output, "; ModuleID = 'advcalc2ir'\ndeclare i32 @printf(i8*, ...)\n@print.str = constant [4 x i8] c\"%%d\\0A\\00\"\n\ndefine i32 @main() {\n");
    //while loop runs until input is null
    while (fgets(str, sizeof(str), input)) {
        lineNumber++;
        // check for comments
        split(str, '%');

        // check '=' if the line is assignment or not
        char* right = split(str, '=');
        
        // means that line is assignment
        if (right){ 
                //left hand side
                char *left, temp; 
                left = strip(str);
                int invalid = 0;
                if (strlen(left) == 0) { //left must not be empty string
                    invalid = 1;
                }
                int len = strlen(left) -1;
                
                // reverse the string
                for (int i = 0; i < len; i++) {
                    temp = *(left+i);
                    *(left+i) = *(left+len);
                    *(left+len) = temp;
                    len--;
                }

                // check validity of variable name
                char* func = isFunction(left);
                for (int i = 0; i < strlen(left); i++) {
                    if (!isalpha(*(left+i))) {
                        invalid = 1;
                        break;
                    }
                }

                if (func || invalid) {
                    printf("Error in line %d!\n", lineNumber);
                    continue;
                }
                

            //evaluate the expression
            right = strip(right); //right hand side
            right = infixToPostfix(right); //convert to postfix
            lli result = evaluateExpression(right); //evaluate
            
            // check if the expression is not empty
            if (lastToken == NONE) {
                printf("Error in line %d!\n", lineNumber);
                continue;
            }

            // check if the expression is valid
            if (right) {
                lli a = result;
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
                char* key = (char *) malloc(sizeof(char)*strlen(left));
                strcpy(key, left);
                setVariable(variables, key, res1);

            } else{
                printf("Error in line %d!\n", lineNumber);
            }

        }else{ // means that line is not assignment
            // evaluate the expression
            char* expr = strip(str);
            expr = infixToPostfix(expr);//convert to postfix
            
            // check validity
            if (expr){
                if (lastToken == NONE) {
                    continue;
                }
                
                lli result = evaluateExpression(expr); //evaluate
                fprintf(output, "\t%lld\n", result);
            }else{
                printf("Error in line %d!\n", lineNumber);
            }
        }
    }
    fprintf(output, "\tret i32 0\n}");
    fclose(input);
    fclose(output);
    return 0;
}

//converts the string from input to postfix notation
char* infixToPostfix(char* str){

    //initialization of all storage variables
    lastToken = NONE;
    Stack* operations = initializeStack();
    Stack* memory = initializeStack(); 
    Stack* functions = initializeStack(); 
    IntStack* commas = initializeIntStack();

    // result string in postfix form
    char* result = (char*) malloc(sizeof(char)*512);
    char* copy = result;

    //characters are grouped, syntax errors are detected due to invalid consecutive token types
    for (; (*str) != '\0'; str++) {
        if (isdigit(*str)) { 
            if (lastToken == VARIABLE || lastToken == NUMBER || lastToken == FUNCTION || lastToken == RIGHT_PARENTHESIS){
                return NULL;
            }
            // add the digit to the memory
            push(memory, *str);
            char next = *(str+1);
            if (isspace(next) || next == ')' || isOperator(next) || next == ',' || next == '\0'){
                // add the memory to the result and clear the memory
                // it is reversed but it is a feature not a bug :)
                while (getSize(memory) > 0) {
                    *result++ = pop(memory);
                }
                *result++ = ' ';
                lastToken = NUMBER;
            } else if (!isdigit(next)){ 
                // if the next character is not digit or space or ')' or operator or digit that means it is error
                return NULL;
            }
            
        } else if (isalpha(*str)) {
            if (lastToken == RIGHT_PARENTHESIS || lastToken == NUMBER || lastToken == VARIABLE || lastToken == FUNCTION){
                return NULL;
            }
            push(memory, *str); //add char to memory
            char next = *(str+1);
            if (next == ')' || next == '(' || isspace(next) || isOperator(next) || next == '\0' || next == ','){
                // add the memory to the result and clear the memory
                char* temp = (char*) malloc(sizeof(char)*getSize(memory)); 
                char* copy = temp;
                while (getSize(memory) > 0) {
                    *temp++ = pop(memory);
                }
                *temp = '\0';

                //handle the operation and comma stacks if token represents a function/operation
                char* func = isFunction(copy);
                if (func) {
                    if (*func == '~'){
                        push(operations, *func);
                        pushInt(commas, 0);
                    }else{
                        pushInt(commas, 1);
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

            } else if (!isalpha(next)){ 
                // if the next character is not letter or space or parenthesis or operator or digit that means it is error
                return NULL;
            }
            

        } else if (isspace(*str)) {
            // ignore the spaces
        } else if (*str == '(') {
            if (lastToken != FUNCTION){
                pushInt(commas, 0);
            }
            // push the '(' to the stack
            push(operations, *str);
            char* next = str+1;
            while (*next == ' '){
                next++;
            }
            if (isOperator(*next) || *next == ')' || lastToken == RIGHT_PARENTHESIS || lastToken == NUMBER || lastToken == VARIABLE) {
                return NULL;
            }
            lastToken = LEFT_PARENTHESIS;
        } else if (*str == ')') {
            if (lastToken == NONE || lastToken == OPERATOR || lastToken == LEFT_PARENTHESIS || lastToken == FUNCTION){
                return NULL;
            }

            int wasComma = popInt(commas);
            if (wasComma>0) { //if a comma was expected, but the paranthesis was closed - error
                return NULL;
            }

            // pop the stack until '('
            // add the popped operators to the result
            char operation = ')';
            while (operation != '(') {  
                if (getSize(operations) == 0){
                    return NULL; // if there is no ( that means it is error
                }
                operation = pop(operations);
                if (operation!='(') {
                    *result++ = operation;
                    *result++ = ' ';
                }
            }

            lastToken = RIGHT_PARENTHESIS;
        } else if (isOperator(*str)) {
            if (lastToken == NONE || lastToken == LEFT_PARENTHESIS || lastToken == OPERATOR || lastToken == FUNCTION){
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
            lli commaCount = popInt(commas);
            if (lastToken == NONE || commaCount <= 0 || getSize(functions)==0 || lastToken == LEFT_PARENTHESIS || lastToken == OPERATOR || lastToken == FUNCTION){
                return NULL; //if invalid token or comma was not expected - error
            }
            pushInt(commas, commaCount-1); //if a comma was expected, great update


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
        return NULL; //if line ends with an invalid token - error

    free(operations);
    free(commas);
    free(memory);
    free(functions);
    *result = '\0';
    return copy;
}

//parse - evaluate from postfix to a single long long int
lli evaluateExpression(char* str){
    if (str) {
        lli power = 1; //variables are held backwards, so start from 1st power
        lli myVar = 0;
        int uninitilized = 1;
        IntStack* res = initializeIntStack();
        while (*str!='\0') {
            if (isdigit(*str)) {
                if (uninitilized) {
                    uninitilized = 0;
                    myVar = 0;
                }
                myVar+=((lli)(*str)-(lli)('0'))*power; //char to int times current power
                power*=10;
                if (*(str+1)=='-') { //handle negative numbers in memory
                    myVar = myVar*-1;
                    str++;
                }
            }
            else if (*str == ' ' && !uninitilized) { //push int to result when see a space                
                pushInt(res, myVar);
                myVar = 0;
                uninitilized = 1;
                power = 1;
            }

            //handle all operations & functions
            else if (*str == '~'){ 
                pushInt(res, ~popInt(res));

            }
            else if (*str != ' '){
                lli a = popInt(res);
                lli b = popInt(res);
                pushInt(res, performOp(b, a, *str));
            }
            str++;
        }
        lli a = popInt(res); //last element in stack is the result
        return a;
        }
    return 0;
}
