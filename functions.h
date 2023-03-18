typedef long long signed int lli;
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
enum tokens {
    NUMBER,
    VARIABLE,
    OPERATOR,
    FUNCTION,
    LEFT_PARENTHESIS,
    RIGHT_PARENTHESIS,
    NONE
};
int isOperator(char str);
char* leftStripper(char* str);
char* rightStripper(char* str);
char* strip(char* str);
char* split(char* str, char find);
Stack* initializeStack();
IntStack* initializeIntStack();
void push(Stack* stack, char str);
void pushInt(IntStack* stack, lli x);

char pop(Stack* stack);
lli popInt(IntStack* stack);
int getSize(Stack* stack);
int getIntSize(IntStack* stack);
lli performOp(lli a, lli b, char c);

int hash(char* str);
char* getVariable(Variable* table, char* key);
void setVariable(Variable* table, char* key, char* value);
char peek(Stack* stack);
int getPrecedence(char* str);
char* isFunction(char* str);

int compare(char* str1, char* str2);