#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* leftStripper(char* str);
char* rightStripper(char* str);


int main(){
  char str[256+1]="";
  printf("> ");
  while (fgets(str, sizeof(str), stdin)) {
    if (str == NULL) {
      break;
    }
    else {
      // check '=' for the line is assignment or not
      char* isAssign = strchr(str, '=');
      if (isAssign){ // means that line is assignment

      *isAssign = '\0'; //ends the string at equal sign

      char* left = str; //left hand side
      char* right= isAssign +1; //right hand side
      left = leftStripper(left);
      left = rightStripper(left);

    //   right = leftStripper(right);
    //   right = rightStripper(right);

        
        if (left) {printf("%s\n", left);}
        else {
          printf("cu");
        }
        printf("%s", left);


      }else{
        //int result


      }

    }
    printf("\n");
    printf("> ");

  }

  return 0;
}