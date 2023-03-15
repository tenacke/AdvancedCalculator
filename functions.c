#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char* leftStripper(char* str) {
    int begin = 0;
    while (*(str+begin) == ' ') {
        begin++;
    }
    return str+begin;
}

char* rightStripper(char* str) {
    char* end = strchr(str, ' ');
    *end = '\0';
    int i=1;
    while (*(end+i) != '\0') {
        if (*(end+i)!= ' ') {
            return NULL;
            break;
        }
        i++;
    }
    return str;

}

// char* strip(char* str) {
//     str = leftStrip(str);
//     str = rightStrip(str);
// }