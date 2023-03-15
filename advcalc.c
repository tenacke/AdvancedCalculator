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
    return str;
}

int* evaluateExpression(char* str){
  return 0;
}

int* getVariable(char* str){
  return NULL;
}

void setVariable(char* str, int value){
}



