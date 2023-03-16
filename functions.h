typedef struct {
  char elems[256];
  int size;
} Stack;
typedef struct {
  char* key;
  char* value;
} Variable;
int isOperator(char str);
char* leftStripper(char* str);
char* rightStripper(char* str);
char* strip(char* str);
char* split(char* str, char find);
Stack* initializeStack();
void push(Stack* stack, char str);
char pop(Stack* stack);
int getSize(Stack* stack);
int hash(char* str);
char* getVariable(Variable* table, char* key);
void setVariable(Variable* table, char* key, char* value);
char peek(Stack* stack);
int getPrecedence(char* str);