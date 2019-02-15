# Toy-FORTH
Toy interpreter for a stack-based FORTH implementation

This was originally based on a class project for my Data Structures course. I rewrote the entire parser and interpreter from scratch. For the project we were given a reverse-Polish notation calculator program with a working parser, and asked to implement the operators described below (`RANDOM`, `INPUT`, and the ability to define functions were not included in the class project specification, I came up with those features on my own).  

The entire language is implemented in postfix notation and utilizes a stack to execute statements. For example, the statement `5 4 +` will place 5 and 4 on the parameter stack, then execute the addition operation when the `+` is encountered, adding 5 and 4 together and placing 9 on the stack.  

#### How the interpreter works:
The parser reads every token in the code and determines what type of token they are. Each token is added to a list of tokens, known as the token buffer. The interpreter reads each token from this list and either adds it to the parameter stack or performs a specific operation.  

Any time a string literal, integer, or unknown token is read from the token buffer, it will be pushed onto the parameter stack. Any time a keyword or operator is encountered, the operation associated with that keyword/operator will be executed. This will typically involve popping tokens from the parameter stack, performing an operation on them, and pushing the results of the operation onto the stack. Keywords, operators, variables, and functions are stored in a symbol table. Keyword and operator symbols will have a function pointer which gets called when the token for that symbol is read by the interpreter.  

Here is an example hello world program in this language:  

  `."Hello, World!" . CR`
  
Any characters in between `."` and `"` are part of a string literal, and get pushed onto the stack as a single token.  
The `.` operator is the print operation. It prints the token on the top of the stack.  
The `CR` operator represents a carriage return, and prints a newline.  
There are even more operators:  

### Arithmetic operations:  
`+`, `-`, `*`, `/`, and `%` are the same arithmetic operators as in C++, only they use postfix notation.  
`NEG` will multiply the item on top of the stack by -1.  

### Printing operations:  
`.` will print the item on top of the parameter stack.  
`CR` will print a new line.  
`SP` will print a space.  

### Stack operations:  
`DUP` will duplicate the item on top of the stack.  
`DROP` will discard the item on top of the stack.  
`SWAP` will swap the positions of the top two items on the stack.  
`ROT` will move the third item to the top of the stack and push the top two items down.  

### Variable operations:  
`SET` will initialize a variable using the item on top of the stack as an identifier and the next item as its value.  
`@` is the "at" or "fetch" operator and is used to access a variable. It will place the value of the variable on the top of the stack.  
`!` is the "store" operator, and is used to set a variable's value to the item on the top of the stack.  

### Relational operators:  
`<`, `<=`, `==`, `!=`, `>=`, and `>` all work the same as in C++, only using postfix notation.  
Boolean values in this language work just like in C/C++, with 0 being false and anything else being true.  

### Logical operators:  
`AND` is the same as "&&" in C++.  
`OR` is the same as "||" in C++.  
`NOT` is the same as "!" in C++.  

### If statement operators:  
`IFTHEN` begins an if statement. Uses the value on top of the stack to determine whether to execute the if or else statement.  
`ELSE` begins the else statement to be executed if the value checked by `IFTHEN` is false.  
`ENDIF` ends an if statement. all `IFTHEN` tokens must have a corresponding `ENDIF`.  

### Loop operators:  
`DO` begins a loop. Loops are executed one or more times.  
`UNTIL` is the ending coniditional for a loop. If the value on the top of the stack is true, the loop will end.  

### Function operators:  
`DEFINE` defines the token on the top of the stack as an identifier for a function.  
`END` ends the function definition.  
Note that "functions" in this language are actually more like macros than functions, as they simply splice any tokens within the function body into the token buffer. However, argument passing can be simulated with some clever stack operations.

### Other keywords:  
`DUMP` will print the entire contents of the parameter stack, and is used for debugging a program.  
`RANDOM` will push a random integer on to the stack. Uses the C++ rand() function.  
`INPUT` will get user input using std::cin and push it onto the stack.  

### Symbols recognized by the parser:
`//` represents a comment, just as in C++.  
`."` begins a string literal and `"` ends the string literal.  

## Example code  
Here is an example program that prints the numbers 0 to 10:  
```
0 i SET             //initialize i to 0
DO                  //begin loop
    i @ . CR            //prints i
    i @ 1 + i !         //increments i
i @ 10 > UNTIL      //until i > 10
```

Here is the same program that uses the stack instead of a variable to hold the value:  
```
0 DO                //begin loop with value of 0
    DUP . CR            //print the value on top of the stack
    1 +                 //increment the item on top of the stack
DUP 10 > UNTIL      //until the value is greater than 10
DROP                //discard the loop value
```
    
