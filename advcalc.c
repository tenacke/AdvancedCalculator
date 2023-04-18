#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "functions.h"

// variables
Variable* variables;
enum tokens lastToken;
// files
FILE* input;
FILE* output;

// evaluate the expression and return NULL if it is not valid
char* evaluateExpression(char* str);

// convert infix expression to postfix expression
char* infixToPostfix(char* str);

//main entrance of the code
int main(int argv, char *args[]){

    //file opening block
    char *FILENAME = args[argv-1];
    size_t length = strlen(FILENAME);
    char* outputFileName = (char*) malloc(strlen(FILENAME)-1);
   strncpy(outputFileName, FILENAME, length-4);
   strcpy(outputFileName+length-4, ".ll");
    int lineNumber = 0;
    char str[256+2] = "";
    input = fopen(FILENAME, "r");
    output = fopen(outputFileName, "w");

    variables = (Variable*) calloc(256, sizeof(Variable)); //variable hashmap initialization

    fprintf(output, "; ModuleID = 'advcalc2ir'\ndeclare i32 @printf(i8*, ...)\n@print.str = constant [4 x i8] c\"%%d\\0A\\00\"\n\ndefine i32 @main() {\n");
    
    //while loop runs until input is null
    while (fgets(str, sizeof(str), input)) {
        lineNumber++;

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
            char* result = evaluateExpression(right); //evaluate
            
            // check if the expression is not empty
            if (lastToken == NONE) {
                printf("Error in line %d!\n", lineNumber);
                continue;
            }

            // check if the expression is valid
            if (right) {
                char* key = (char *) calloc(strlen(left)+1, 1);
                char* copy = key;
                *key++ = '%';
                int length = strlen(left);
                for (int i = length-1; i >= 0 ; i--) {
                    *key++ = *(left+i);
                }
                copy = strip(copy);
                setVariable(variables, copy, result);
            } else{
                printf("Error in line %d!\n", lineNumber);
            }
            free(result);
            free(right);
        }else{ // means that line is not assignment
            // evaluate the expression
            char* expr = strip(str);
            expr = infixToPostfix(expr);//convert to postfix
            // check validity
            if (expr){
                if (lastToken == NONE) {
                    continue;
                }
                char* result = evaluateExpression(expr); //evaluate
                fprintf(output, CALL, result);
                free(result);
            }else{
                printf("Error in line %d!\n", lineNumber);
            }
            free(expr);
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
    Stack* commas = initializeStack();

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
                char* temp = (char*) calloc(getSize(memory), sizeof(char)); 
                char* copy = temp;
                char* verycopy = temp;
                while (getSize(memory) > 0) {
                    *temp++ = pop(memory);
                }
                *temp = '\0';

                //handle the operation and comma stacks if token represents a function/operation
                char* func = isFunction(copy);
                if (func) {
                    if (*func == '~'){
                        push(operations, *func);
                        push(commas, 0);
                    }else{
                        push(commas, 1);
                        push(functions, *func);
                    }
                    lastToken = FUNCTION;
                } else {
                    if (!getVariable(variables, copy)){
                        return NULL;
                    }
                    while (*copy != '\0') {
                        *result++ = *copy++;
                    }
                    *result++ = '%';
                    *result++ = ' ';
                    lastToken = VARIABLE;
                }
                free(verycopy);

            } else if (!isalpha(next)){ 
                // if the next character is not letter or space or parenthesis or operator or digit that means it is error
                return NULL;
            }
            

        } else if (isspace(*str)) {
            // ignore the spaces
        } else if (*str == '(') {
            if (lastToken != FUNCTION){
                push(commas, 0);
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

            int wasComma = (int) pop(commas);
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
            int commaCount = pop(commas);
            if (lastToken == NONE || commaCount <= 0 || getSize(functions)==0 || lastToken == LEFT_PARENTHESIS || lastToken == OPERATOR || lastToken == FUNCTION){
                return NULL; //if invalid token or comma was not expected - error
            }
            push(commas, 0); //if a comma was expected, great update


            // use the comma as an operator for a function
            char op = pop(functions);

            //operation precedence handling block
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

//parse - evaluate the postfix, call lli printing, return stored or printed register

char* evaluateExpression(char* str){
    if (str) {
        Stack* temp = initializeStack();
        PStack* res = initializePStack();
        while (*str!='\0') {
            if (isdigit(*str) || isalpha(*str)) {
                push(temp, *str);

            }
            else if (*str == '%' && getSize(temp) > 0){ //load an already saved variable and push it to stack
                char* elem = (char *) calloc((getSize(temp)+2), sizeof(char));
                char* copy = elem;
                *copy++ = '%';
                while (getSize(temp) > 0){
                    *copy++ = pop(temp);
                }
                *copy++ = '\0';
                char* reg = getNewRegister();
                fprintf(output, LOAD, reg, elem);
                free(elem);
                pushP(res, reg);
            }
            else if (*str == ' ' && getSize(temp) > 0) { //push a new pointer to result when see a space
                char* elem = (char *) calloc((getSize(temp)+1), sizeof(char));
                char* copy = elem;
                while (getSize(temp) > 0){
                    *copy++ = pop(temp);
                }
                *copy++ = '\0';

                pushP(res, elem);
            }
                //handle all operations & functions
            else if (*str == '~'){
                char* a = popP(res);
                pushP(res, performOp(a, "-1", '^'));
                free(a);
            }
            //next two blocks handle rotations 
            else if (*str == '@'){
                char* b = popP(res);
                char* a = popP(res);
                char* c = performOp("32", b, '-');
                char* d = performOp(a, b, '!');
                char* e = performOp(a, c, '$');
                pushP(res, performOp(d, e, '|'));
                free(a);
                free(b);
                free(c);
                free(d);
                free(e);
            }
            else if (*str == '#'){
                char* b = popP(res);
                char* a = popP(res);
                char* c = performOp("32", b, '-');
                char* d = performOp(a, b, '$');
                char* e = performOp(a, c, '!');
                pushP(res, performOp(d, e, '|'));
                free(a);
                free(b);
                free(c);
                free(d);
                free(e);
            }
            else if (*str != ' '){ //handle other functions/operations
                char* b = popP(res);
                char* a = popP(res);
                pushP(res, performOp(a, b, *str));
                free(a);
                free(b);
            }
            str++;
        }
        char* a = (char *) malloc(sizeof peekP(res));
        strcpy(a, popP(res)); //last element in stack is the resulting register/number
        free(temp);
        free(res);
        return a;
    }
    return NULL;
}