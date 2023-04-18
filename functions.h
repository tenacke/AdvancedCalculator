// macros
#define ALLOCA  "\t%s = alloca i32\n"
#define STORE  "\tstore i32 %s, i32* %s\n"
#define CALL  "\tcall i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %s)\n"
#define LOAD  "\t%s = load i32, i32* %s\n"
#define XOR  "\t%s = xor i32 %s, %s\n"
#define LSHIFT  "\t%s = shl i32 %s, %s\n"
#define RSHIFT  "\t%s = lshr i32 %s, %s\n"
#define ADD  "\t%s = add i32 %s, %s\n"
#define SUB  "\t%s = sub i32 %s, %s\n"
#define MUL  "\t%s = mul i32 %s, %s\n"
#define DIV  "\t%s = sdiv i32 %s, %s\n"
#define AND  "\t%s = and i32 %s, %s\n"
#define OR  "\t%s = or i32 %s, %s\n"
#define MOD  "\t%s = srem i32 %s, %s\n"


//some new type definitions
typedef struct {
  char elems[256];
  int size;
} Stack;
typedef struct {
    char *elems[256];
    int size;
} PStack;
typedef struct {
  char* key;
} Variable;

//token types
enum tokens {
    NUMBER,
    VARIABLE,
    OPERATOR,
    FUNCTION,
    LEFT_PARENTHESIS,
    RIGHT_PARENTHESIS,
    NONE
};

//generally used functions
int isOperator(char str);
char* leftStripper(char* str);
char* rightStripper(char* str);
char* strip(char* str);
char* split(char* str, char find);
int compare(char* str1, char* str2);

//stack definitions and functions
Stack* initializeStack();
PStack* initializePStack();
void push(Stack* stack, char str);
char pop(Stack* stack);
int getSize(Stack* stack);
char peek(Stack* stack);
void pushP(PStack* stack, char* str);
char* popP(PStack* stack);
int getSizeP(PStack* stack);
char* peekP(PStack* stack);

//functions connected to parsing and variable handling
char* performOp(char* a, char* b, char c);
int hash(char* str);
int getVariable(Variable* table, char* key);
void setVariable(Variable* table, char* key, char* value);
int getPrecedence(char* str);
char* isFunction(char* str);
char* getNewRegister();
