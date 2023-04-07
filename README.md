AdvCalc CMPE230
Emre Kılıç 2021400015, Kristina Trajkovski 2020400384
Submission Date: March 28th, 2023

Introduction
AdvCalc is a programming project written in C by Emre Kılıç and Kristina Trajkovski for CMPE230 in March 2023. It runs in the terminal and allows users to perform algebraic and boolean operations such as addition, subtraction, multiplication, bitwise AND, OR, XOR, NOT, shifts, and rotations.

We (Emre and Kristina) tackled this challenge by reading the input from the user (from the terminal directly), attempting to convert the expression line into postfix notation, parsing the postfix notation, and returning a value or storing it. We implemented new structures and functions such as HashMap and Stack which will be described in more detail below. Also, an abbreviation that will be used throughout the document is lli for long long signed integer. 

How to Use:
	Step 1:An executable file is made by typing “make” into the terminal while in the appropriate folder. 
	Step 2: The executable is run by typing ./advcalc into the terminal.
	Step 3: Lines of input should be written according to the rules described below in the next segment. This code may be used as an advanced calculator. Operations that are supported are addition, subtraction, multiplication, bitwise AND, bitwise OR, bitwise XOR, left and right shifts, left and right rotation, and NOT. All of their combinations are valid as well. Variables may be stored by standard assignment lines (x = a+b etc.). For a result display, the line must not contain ‘=’.
	Step 4: Once done, Ctrl+D to exit from the calculator.
Rules
To be able to use the Advance Calculator:
The input message should not exceed 256 characters.
Integer values should not exceed the standard 64-bit boundaries.

Not to receive an error:
Negative integers should not be written directly into the terminal. However, variables that have negative values may be stored (ex. x=-1 is invalid, but may store x as x=2-3).
Variables with function names should not be defined (xor, ls, rr, etc.).
All functions except ‘not’ should be written alongside parentheses with exactly 2 arguments separated by a comma ‘,’.
Each open parenthesis should be closed
The ‘%’ character should be used only for comments
More than one ‘=’ should not be used in assignments
The order of digits and operations between them should follow the infix notation (ex. 5 5 + is not a valid order for this code, use 5+5)
Variables should be created only by using upper and lowercase letters of the English alphabet
There should be no digits on the left side of an assignment
Parentheses should be used only on the right end side of an assignment
Every set of parentheses should contain a variable or an integer -ex. ((())) is invalid, but (((5)))+6 is valid
Commas should be written inside a parentheses pair only
Do not use square or curly brackets

Implementation
-advcalc.c
This file contains three major functions of this code:
int main() that handles the input, decides if the input is a variable assignment or just an operation expecting an immediate output, and prints the appropriate output
infixToPostFix() that takes the input in infix notation, pulls appropriate variables from storage if any, checks for errors, and converts the given input into postfix notation
evaluateExpression() that takes a string (char pointer) in postfix notation, parses, and returns the resulting long long integer value

-functions.h
This file contains new structure definitions (Stacks and HashMap variables), defines our abbreviation lli, enum token definitions, and all of the other function headers.
-functions.c
This file contains all of the remaining functions such as hashTable initialization; variable placement and retrieving; character type recognition; string comparison, splitting, and stripping; stack initializations, pushing, popping, and peeking; operation precedence determination, and final operation performing

-libraries included
<stdio. h>
<stdlib.h>
<string.h>
<stdbool.h>
<ctype.h>

Newly Defined Structures
Stack / IntStack:
-A collection of elements of type char or lli which pushes to and pops from the tail (last element) only. 

-Related functions:
Stack* initializeStack() / IntStack*  initializeIntStack(): 
allocates memory for the stacks by using malloc and returns a pointer to it
void push(*Stack stack, char str) / void pushInt(*IntStack stack, lli x) : 
takes the targeted stack and element (char or lli) as parameters and pushes the element to the end of the given stack. 
char pop(*Stack stack) / lli popInt(*IntStack stack):
takes a stack as a parameter, removes the last element from the given stack, and returns the element removed
char peek(*Stack stack):
takes a stack as a parameter and returns the last element (without altering the stack)
int getSize(Stack* stack) / int getIntSize(IntStack* stack):
takes a stack as a parameter, returns the number of elements in the stack

In our code, we use 5 stacks in total. The 4 character stacks which are used in infix to postfix conversion are operations (stores only the default operators and the not operation - +, -, *, ~), memory (holds characters until a token is created - ex holds 1234 as ‘4’, ‘3’, ‘2’, ‘1’ until a non-integer value is seen and creates a token; same goes for letters and variable creation), functions (stores our assigned function representations described in isFunction() ), and commas (checks for comma related errors, stores a value each time a parenthesis is opened - 0 if a comma is not expected, and 1 if it is, the value is popped once a comma or closed parentheses appear). The usage of the IntStack is described as a part of evaluateExpression() in the ”From Input to Output” section. 

HashMap
A collection of elements of newly defined type Variable (holds a key - name, and a value - chars containing digits and possibly a minus sign only)
Allocates memory for the collection by using malloc
Stores and returns a value at an index returned by a hash function
Handles conflicts by using linear probing
Related functions:
int hash(char* str) : 
takes the name of a variable as a parameter, returns the sum of the ASCII values of the name characters as an index (mod 256 to make sure the index is not out of bounds)
char* getVariable(Variable* table, char* key) : 
takes a HashMap table and a wanted key name as parameters, calls the hash function to see the potential index of the key, checks the index or searches linearly if the index is occupied by a wrong name, returns 0 if the name has not been found or returns the value if it has
void setVariable(Variable* table, char* key, char* value) : 
takes a HashMap table, a key name, and a value as parameters, calls the hash function to see the potential index of the key, checks if the index is occupied by an unwanted key, and moves forward linearly if so, updates the value if the key is already in the table, or places the key and value at an empty index

HashMap was the hardest structure to implement, for it depends on many functions including memory allocation, the hash function itself, string comparisons (which taught us not to trust functions that just seem familiar, read the documentation, or implement our own string comparator function), storing and retrieving variables from memory (which definitely taught us a lot about pointers and how they are affected by new inputs).

-Other functions:
String functions : 
int compare(char* str1, char* str2):
takes two strings (character pointers) as parameters, compares all characters, returns 0 as soon as it finds a difference, and returns 1 if no differences have been found
char* leftStripper(char* str) : 
takes a string (char pointer) as a parameter, starting from the first character, increments the pointer if the characters are empty spaces, returns the new string (incremented char pointer) without leading spaces
char* rightStripper(char* str) : 
takes a string (char pointer) as a parameter, starting from the last character, replaces empty spaces with the terminal string character (‘\0’), returns the new string (char pointer) without spaces at the end
char* strip(char* str) : 
takes a string (char pointer) as a parameter, calls the leftStripper and rightStripper functions described above and returns a string (char pointer) without empty spaces at the beginning or end
char* split(char* str, char find) : 
takes a string (char pointer) and a wanted splitting character as parameters, replaces the first occurrence of the “find” character with the terminal character (‘\0’) and returns a pointer to the second part of the split string (char pointer to the character after ‘\0’)

Character functions:
char* isFunction(char* str):
takes a string (char pointer) as a parameter, returns a character representing the wanted operation if the given string matches an operation keyword (‘^’ for xor, ‘!’ for right shift, ‘$’ for left shift, ‘@’ for left rotation, ‘#’ for right rotation, ‘~’ for NOT operation). 
int getPrecedence(char* str):
takes a character representing an operation as a parameter, returns an int representing the precedence in our code with 1 being the lowest (functions that require parentheses in the first place), and 7 being the highest (the NOT operator)
int isOperator(char* str):
takes a character representing an operation as a parameter, checks if it is one of the operations that does not require parentheses (addition, subtraction, multiplication, bitwise AND, or bitwise OR), returns 1 if it is an operation, returns 0 otherwise
lli performOp(lli a, lli b, char c) : 
takes two lli’s and a character representing an operation as parameters, returns the resulting lli of the operation represented by c between a and b

Token Type Checking:
Checking if two consecutive token types are valid, altering memory, and generation of new tokens if necessary
Note: characters and numbers are stored in a “memory” stack, out of them tokens are generated when a space, left parenthesis, right parenthesis, or an operator comes and memory is emptied

-FROM INPUT TO OUTPUT
(All of the minor functions and structures are explained in more detail above)
	
Before any lines are read a new HashMap is allocated
A line is read from input (code stops here if the input is CTRL + D)
The line is split where the ‘%’ character begins if any (comments are ignored)
The line is split at the ‘=’ character if any (assignment or expression)
In case the line is an assignment the validity of the left-hand side is checked (letters are allowed only)
The right-hand side or the expression line itself is sent to infixToPostfix() function
In infixToPostfix function the line is iterated character by character, errors are checked according to the table above (they return NULL immediately), tokens are generated according to the table above, tokens of variables are created, variables are replaced with their value returned by getVariable() function, number tokens are created, number and variable tokens are stored in final postfix line by occurrence and in reverse (this may seem like a bug, but it is a feature that makes parsing easier later), operations and functions are stored in stack by precedence returned by getPrecedence() and the rule that an operation of lower precedence cannot come on top of a stronger operation (in that case the operation stack is emptied and written to the final postfix line), operation and memory stacks are emptied similarly if a closed parenthesis occurs, before the end all stacks are emptied and a final postfix line is returned
The resulting postfix line from step 7 is sent to evaluateExpression() function unless it was NULL, numbers are parsed by adding up the digit with the power of its (index+1) in the token (ex. 56482 is actually 5*1+ 6*10+ 4*100+ 8*1000+2*10000 = 28465), the resulting lli is multiplied with -1 in case the token is followed by a minus sign without a space in between, lli’s are stored in an IntStack, when an operation is seen, two lli’s are popped from the stack and the operation is performed by performOp(), the result from performOp() is added to the stack, the last remaining element in the stack is the final result returned
In case the step 4 determined that the line was an assignment, the result from evaluateExpression() is assigned to (or updated) the variable name at the left hand side by using setVariable(), nothing is printed. Otherwise, if the line was just an expression the result from evaluateExpression() is printed. Now the program goes back to step 2 for later inputs.

Difficulties Encountered and Improvements

-Finding the right algorithm

	The first difficulty that we faced was getting to know C. So far, we have been programming in high-level languages which have many structures and functions implemented already. We spent a lot of time learning about pointers, dealing with the infamous “Segmentation fault” in places we would not even think of, and implementing many functions and structures such as stack, hashmap, split, strip etc. from scratch. However, this situation gave us full control over all functions and structures, all input, output and memory were in our hands and we learned a lot about what is going on on the levels of single characters.
	Then, when we first started brainstorming we could not come up with an algorithm right away. Since we have learned BNF in another class that was our first idea, however, we started thinking of all the checks we would have to do to make it work. Also, the complexity of recursion with each iteration and each operation seemed more complicated to calculate. Quickly we decided on postfix notation conversion when we remembered how easy it was to determine precedence with it and how we would complete a line evaluation in two iterations only. But on the bad side, postfix notation required a stack implementation, and in our code, 5 stacks in total. Still, we ended up using the postfix notation conversion and debugging seemed to go much easier. 
	The last major element that required brainstorming was variable storage. At the time, we had not learned of malloc or calloc for memory allocation and struct for defining our own structure type. With experience from previous years, we decided that the fastest and best way to store was a HashMap, and although there is no short way to do that, we will did our best to implement it from scratch. It ended up being the structure that brought the most bugs into our code, but it may have taught us the most about memory allocation, pointer returning and string comparisons in C.


-Errors worth of mention (the biggest bugs)

After implementing our HashMap, we had to test if getting the variable from the map was valid. At some point, if a variable was already initialized a correct value was returned, but each time we would not find it, instead of returning 0 we would receive a segmentation fault. Merely we had a line that said : 
	if (compare((table+index)->key, key) return (table+index)->value;
	else return 0;
Which means that in case the iteration stopped at the wanted key, the stored value will be returned. However we overlooked that if the wanted key has not been stored, our iteration stops at key NULL. Comparing it with any keys is then impossible and Segmentation fault error is thrown right away with no other explanation. After fixing it, we check the NULL condition first and return 0 if such, and return the stored value otherwise.

When converting the line to postfix notation, the program analyzes the line character by character. In order to parse an integer or variable, previous characters are stored in a Stack. At the time characters of the same type stop appearing, a backtracking mechanism pops the characters from stack to postfix notation. It was a clever approach since it helped us with parsing the string in nearly O(N) time, but it came with a handicap- the variables and the numbers were in reverse. It was a huge bug and it seemed like it required a lot of time to solve. The solution would also make the algorithm more complex. After some brainstorming, we realized that it was not a problem at all. Storing all variables in reverse format does not affect the program flow. Storing the numbers in reverse format is also not a problem but a helpful approach. Parsing a reverse number is even easier. For example “15” would be parsed by 1*10+5*1. In order to do this we need to know the length of the string to start multiplying the first character with a proper power of the radix. Another idea is reversing the string and starting multiplying with one and increasing the power of the radix accordingly. We choose the second idea and we have the variable and integer strings already reversed.

Another error was thinking of the comma as an operator. The program handles functions except “not” in the following way: firstly, the program parses the name of the function and assigns the character returned by isFunction() to it. Then, it adds the unique character in a stack and waits for the comma. When the program encounters a comma, it pops an operation from the stack and handles the rest just like handling the predefined operators. It works well at a point, it tracks the equality of number of commas and number of functions. But here is the bug: The program does not check whether the comma divides the function block into two halves or not. For example, “ls((1,1))” thinking comma as an operator handles this function and creates the postfix string as “1 1 $“. But it is incorrect since the comma does not divide the function block, there is only one invalid argument in this example. Our best friend Stack came to our rescue one more time. After every left parenthesis, we pushed 0 or 1 to the stack and every time we encountered a right parenthesis, we popped one. If the parenthesis belongs to a function then the value would be 1, and 0 otherwise. In every comma, we subtracted 1 from the peak value of the stack. We did our checks in the pop part - if the popped value was not 0, then the format would be invalid. 

Another error was working on the same address block for strings. The initial input line is stored in a string array. When parsing the line to a postfix string, the detected variables are directly written to a HashMap that we implemented before. But then a bug occurred. When a user enters a new input, the new line is stored in the same address block. Because of this behavior, our previously stored variables were then pointing at a different string. The solution of this bug was simple compared to others, we just copied the variable string to another address block and solved the bug.

Conclusion

	As we submit our first project in C, we can most definitely say that AdvCalc taught us a lot - from memory and pointer allocations to parsing and analyzing. We have learned that there is no such thing as a small unimportant function, for implementing each and every one of them was needed, required time and dedication, and definitely caused at least one bug which taught us something new. We have also learned not to take any structures for granted because implementing them and their related functions was a piece of work as well, but made the major functions carrying the code itself more readable and easier to implement. At the end of the day, after blood sweat and tears shed, we seem to have a fully functioning advanced calculator in our hands. 




