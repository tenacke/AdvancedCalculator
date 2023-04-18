// macros
#define ALLOCA = "%s = alloca i32"
#define STORE = "store i32 %s, i32* %s"
#define CALL = "call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %s)"
#define LOAD = "%s = load i32, i32* %s"
#define XOR = "%s = xor i32 %s, %s"
#define LSHIFT = "%s = shl i32 %s, %s"
#define RSHIFT = "%s = lshr i32 %s, %s"
#define NOT = "%s = xor i32 %s, -1"
#define ADD = "%s = add i32 %s, %s"
#define SUB = "%s = sub i32 %s, %s"
#define MUL = "%s = mul i32 %s, %s"
#define DIV = "%s = sdiv i32 %s, %s"
#define AND = "%s = and i32 %s, %s"
#define OR = "%s = or i32 %s, %s"
#define MOD = "%s = srem i32 %s, %s"


//some new type definitions
typedef long long signed int lli;
typedef struct {
  char elems[256];
  int size;
} Stack;
typedef struct {
  lli elements[256];
  int size;
} IntStack;
typedef struct {
  char* key;
  char* value;
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
IntStack* initializeIntStack();
void push(Stack* stack, char str);
void pushInt(IntStack* stack, lli x);
char pop(Stack* stack);
lli popInt(IntStack* stack);
int getSize(Stack* stack);
int getIntSize(IntStack* stack);

//functions connected to parsing and variable handling
lli performOp(lli a, lli b, char c);
int hash(char* str);
char* getVariable(Variable* table, char* key);
void setVariable(Variable* table, char* key, char* value);
char peek(Stack* stack);
int getPrecedence(char* str);
char* isFunction(char* str);
char* getNewRegister();
