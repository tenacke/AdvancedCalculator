#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "functions.h"

// operators
char* XOR = "xor";
char* RS = "rs";
char* LS = "ls";
char* RR = "rr";
char* LR = "lr";
char* NOT = "not";

// check if the variable is defined and return its value
// if not return NULL
int* getVariable(char* str);

// set the variable with the value
void setVariable(char* str, int value);

// evaluate the expression and return NULL if it is not valid
int* evaluateExpression(char* str);

// convert infix expression to postfix expression
char* infixToPostfix(char* str);

int main(){
  char str[256+1] = "";
  printf("> ");
  while (fgets(str, sizeof(str), stdin) && str != NULL) {
       // check for comments
       split(str, '%');

      // check '=' for the line is assignment or not
      char* right = split(str, '=');
      
      if (right){ // means that line is assignment
        char* left = strip(str); //left hand side
        
        //evaluate the expression
        right = strip(right); //right hand side
        right = infixToPostfix(right); //convert to postfix
        int* result = evaluateExpression(right);

        // check if the expression is valid and set the variable
        if (result){
          setVariable(left, *result);
        }else{
        printf("Error!");
        }

      }else{ // means that line is not assignment
        // evaluate the expression
        char* expr = strip(str);
        expr = infixToPostfix(expr);
        int* result = evaluateExpression(expr);
        if (result){
          printf("%d", *result);
        }else{
        printf("Error!");
        }
      }
    printf("\n");
    printf("> ");
  }
  return 0;
}

char* infixToPostfix(char* str){
    Stack* operations = initializeStack();
    // memory for digits and letters
    Stack* memory = initializeStack(); 
    // result string in postfix form
    char* result = (char*) malloc(sizeof(char)*256);
    for (; (*str) != '\0'; str++) {
        if (isdigit(*str)) {
            // add the digit to the memory
            push(memory, *str);
            char next = *(str+1);
            if (isspace(next) || next == ')' || isoperator(next)){
                // add the memory to the result and clear the memory
                while (getSize(memory) > 0) {
                    *result++ = pop(memory);
                }
                *result++ = ' ';
            } else if (!isdigit(next)){ 
                // if the next character is not digit or space or ')' or operator or digit that means it is error
                return NULL;
            }
        } else if (isalpha(*str)) {
            push(memory, *str);
            char next = *(str+1);
            if (isspace(next) || next == '(' || next == ')' || isoperator(next)){
                // add the memory to the result and clear the memory
// TODO handle functions here
                char* temp = (char*) malloc(sizeof(char)*getSize(memory)); 
                while (getSize(memory) > 0) {
                    *temp++ = pop(memory);
                }
                *temp++ = ' ';
// TODO temp is pointing end of the letter fix this
            } else if (!isalpha(next)){ 
                // if the next character is not letter or space or parenthesis or operator or digit that means it is error
                return NULL;
            }
        } else if (isspace(*str)) {
            // add the memory to the result
            // clear the memory
        } else if (*str == '(') {
            // push the '(' to the stack
            push(operations, *str);
        } else if (*str == ')') {
            // pop the stack until '('
            // add the popped operators to the result
            char operation = pop(operations);
            while (operation != '(') {
                if (getSize(operations) == 0)
                    return NULL; // if the stack is empty that means it it error
                *result++ = operation;
                *result++ = ' ';
                operation = pop(operations);
            }

        } else if (isoperator(*str)) {
            // pop the stack until top of the stack has lower precedence than the operator
            
            if (getSize(operations) == 0){
                push(operations, *str);
            }
            // push the operator to the stack
            
        } else if (*str == ','){
// TODO look up functions
        } else {
            // if the character is not valid that means it is error
            return NULL;
        }
    }
    return result;
}

int* evaluateExpression(char* str){
    if (str) {
        
    }
    return NULL;
}

int* getVariable(char* str){
  return NULL;
}

void setVariable(char* str, int value){
}

