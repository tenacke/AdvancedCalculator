typedef struct {
  char elems[256];
  int size;
} Stack;
char* leftStripper(char* str);
char* rightStripper(char* str);
char* strip(char* str);
char* split(char* str, char find);
Stack* initializeStack();
void push(Stack* stack, char str);
char pop(Stack* stack);
int getSize(Stack* stack);