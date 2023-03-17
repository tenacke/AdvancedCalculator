typedef struct {
  char elems[256];
  int size;
} Stack;
typedef struct {
  int elements[256];
  int size;
} IntStack;
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
IntStack* initializeIntStack();
void push(Stack* stack, char str);
void pushInt(IntStack* stack, int x);

char pop(Stack* stack);
int popInt(IntStack* stack);
int getSize(Stack* stack);
int getIntSize(IntStack* stack);
int performOp(int a, int b, char c);

int hash(char* str);
char* getVariable(Variable* table, char* key);
void setVariable(Variable* table, char* key, char* value);
char peek(Stack* stack);
int getPrecedence(char* str);
char* isFunction(char* str);
