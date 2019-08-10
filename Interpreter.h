#ifndef INTERPRETER_H
#define INTERPRETER_H

//TODO: DESCRIBE THE PROGRAM, HOW IT WORKS, WHAT FORTH IS, STRING LITERALS, ETC...

#include <stack>
#include <list>
#include <map>
#include <string>
#include <fstream>
#include "Symbol.h"

class Interpreter
{
public:
	//Constructor for the interpreter.
	//Argument is the name of the text file containing the FORTH code.
	//Parses the text file, then calls mainLoop().
	Interpreter(const std::string& fileName);

private:
	//The main execution of the interpreter.
	//Repeatedly calls nextToken() until the list of tokens isempty or an 
	//exception is thrown. (TODO: DESCRIBE WHY AN EXCEPTION MIGHT BE THROWN)
	void mainLoop();

	//Returns the next token from the list of tokens
	Token nextToken();

	//Removes and returns the item on the top of the parameter stack.
	//Does not check if the stack is empty, as each function should first
	//check if there are enough items on the stack before using this.
	Token popStack();

	//Returns true if the token is in the symbol table.
	bool isSymbol(const Token& t);

	//Checks the number of items on the stack before an operation.
	//If the stack has less items than minItems, throw an underflow_error
	//exception. The exception will print a message stating the required
	//number of items and the operation name.
	void checkStackSize(const unsigned int minItems, const std::string& operationName);

	//Debugging function that prints the contents of the symbol table.
	void printSymTab();

	//Debugging function that prints the contents of tkBuffer.
	void printTokenBuffer();

	//Print the size of the stack at the end of execution.
	void printEndInfo();

	//map for symbols
	//Holds information on all the keywords/operators, user-defined functions and variables
	std::map<std::string, Symbol> symTab;

	//parameter stack
	//All tokens that are not keywords/operators are pushed onto
	//the stack, including variables.
	std::stack<Token> params;

	//List of tokens read by the parser.
	//Every token encountered in the input file will be added to this list.
	std::list<Token> tkBuffer;

	//Stack of Lists of tokens found within a loop, each list represents
	//a nested loop, with the innermost loop being on the top of the stack.
	std::stack<std::list<Token>> loopBuffers;

	//File opened with the OPEN keyword and read with the READ keyword.
	std::ifstream file;

	//DEBUG: FOR TESTING PARSER
	//stack<string> debugStack;






	/*----------------------------------------------------------------------
	  Functions belonging to keyword/operator symbols:
	
	  Each function is static to simplify the use of function pointers.
	  Because they are static, each function must take a pointer to the
	  Interpreter as an argument so they can access the parameter stack.
	
	  Any time a keyword/operator symbol is encountered during mainLoop(),
	  the interpreter will call its function and pass itself as the argument.
	----------------------------------------------------------------------*/


	//Debug operation: prints the entire parameter stack (without changing it)
	static void doDUMP(Interpreter *iptr);




	//Arithmetic operations:

	//Preconditions: must have at least 2 items on the stack.
	//Postconditions: does the operation on the top 2 items of the stack and
	//pushes the results onto the stack.
	//Results are computed using Reverse Polish Notation,
	//ie: "7 5 -" equals 2.
	static void doPlus(Interpreter *iptr);		// +
	static void doMinus(Interpreter *iptr);		// -
	static void doMultiply(Interpreter *iptr);	// *
	static void doDivide(Interpreter *iptr);	// /
	static void doMod(Interpreter *iptr);		// %

	//Multiplies the value on top of the stack by -1.
	//Precondition: stack must have at least 1 item.
	static void doNEG(Interpreter *iptr);




	//Printing operations:

	//"." operator
	//Prints (and discards) the item on the top of the stack.
	//Precondition: stack must have at least 1 item.
	static void doDOT(Interpreter *iptr);
	
	//Carriage Return: Prints a new line.
	//Equivalent to printing "\n".
	static void doCR(Interpreter *iptr);

	//Prints a space.
	static void doSP(Interpreter *iptr);





	//Stack operations:
	
	//Duplicates the item on top of the stack, pushing its copy onto the stack.
	//Precondition: stack must have at least 1 item.
	static void doDUP(Interpreter *iptr);

	//Deletes the item on top of the stack.
	//Precondition: stack must have at least 1 item.
	static void doDROP(Interpreter *iptr);

	//Swaps the first and second items on top of the stack.
	//ie: "1 2 SWAP" is the same as "2 1".
	//Precondition: stack must have at least 2 items.
	static void doSWAP(Interpreter *iptr);

	//Rotates the third item from the top of the stack to be on top,
	//and pushes down the top two items to be below it.
	//ie: "1 2 3 ROT" is the same as "2 3 1".
	//Precondition: stack must have at least 3 items.
	static void doROT(Interpreter *iptr);





	//Variable/array operations:

	//Initializes a variable and adds it to the symbol table.
	//Precondition: stack must have at least 2 items.
	//If the variable already exists, print an error message and move on.
	static void doSET(Interpreter *iptr);

	//"@" operator
	//Returns the value held by that variable.
	//Precondition: stack must have at least 1 item.
	//If the variable does not exist, print an error message and move on.
	static void doAt(Interpreter *iptr);

	//"!" operator
	//Stores a value in the variable on top of the stack.
	//Precondition: stack must have at least 2 items.
	//If the variable does not exist, print an error message and move on.
	static void doStore(Interpreter *iptr);


	//Allocates an int array of the specified size.
	//Precondition: stack must have at least 2 items.
	//Usage: '10 arr ALLOT' creates an array named arr with size 10.
	static void doALLOT(Interpreter *iptr);

	//"#@" operator
	//Accesses an index of an array and places that value on top of the stack
	//Precondition: stack must have at least 2 items.
	//Usage: 'arr 4 #@' accesses index 4 of an array named arr.
	//If the the array does not exist, print an error message and move on
	//If the index is out of bounds, print an error message and move on
	static void doArrayAt(Interpreter *iptr);

	//"#!" operator
	//Stores an int value at the specified index of an array.
	//Precondition: stack must have at least 3 items.
	//Usage: '25 arr 2 #!' stores the value 25 at index 2 of array 'arr'
	//If the the array does not exist, print an error message and move on
	//If the index is out of bounds, print an error message and move on
	static void doArrayStore(Interpreter *iptr);



	//Comparison operations:
	
	//Compares the values of 2 tokens and pushes the result onto the stack. 
	//Preconditions: stack must have at least 2 items.
	//Compares the tokens in normal order, 
	//i.e. "3 10 <" returns true and "3 10 >" returns false.
	static void doLessThan(Interpreter *iptr);
	static void doLessThanOrEqual(Interpreter *iptr);
	static void doEqualTo(Interpreter *iptr);
	static void doNotEqual(Interpreter *iptr);
	static void doGreaterThanOrEqual(Interpreter *iptr);
	static void doGreaterThan(Interpreter *iptr);





	//Logic operations:

	//Equivalent to C++ logic operators.
	//Just as in C++: 0 is false, everything else is true.
	//AND = &&
	//OR = ||
	//NOT = !
	static void doAND(Interpreter *iptr);
	static void doOR(Interpreter *iptr);
	static void doNOT(Interpreter *iptr);





	//Conditional operations:

	//IFTHEN will check the value on the top of the stack as a boolean condition.
	//If it is true, the ifthen block will be executed, if not, then the
	//interpreter will skip over to the ELSE block and execute that.
	//IFTHEN/ELSE statements should always end with an ENDIF token.
	static void doIFTHEN(Interpreter *iptr);
	//static void doELSE(Interpreter *iptr);





	//Loop operations:

	//DO will execute all code found between DO and UNTIL.
	//When UNTIL is reached, the value on top of the stack will be checked
	//as a boolean. If true, execution of the loop ends. If false, the loop
	//will be executed again, and keep executing until the UNTIL returns true.
	static void doDO(Interpreter *iptr);
	static void doUNTIL(Interpreter *iptr);




	//Function operations:
	//Defines the token on top of the parameter stack.
	//Every token encountered until END will be put into a list of tokens.
	//When the function name is found in the code, the list of tokens within
	//the definition will be spliced into the beginning of the token buffer.
	static void doDEFINE(Interpreter *iptr);




	//Random number generator:
	//Returns a random number using rand() and places it on the parameter stack.
	static void doRANDOM(Interpreter *iptr);




	//Get user input:
	//Gets integer input from the user and places it on the parameter stack.
	static void doINPUT(Interpreter *iptr);




	//File I/O:

	//Opens a file for reading.
	//Precondition: The parameter stack must have at least 1 item, which should 
	//be a string literal.
	static void doOPEN(Interpreter *iptr);

	//Reads a single char from an open file as an ASCII value.
	//Precondition: a file must be open.
	//Places the char from the file onto the parameter stack as in int using its 
	//ASCII value.
	static void doREAD(Interpreter *iptr);
};

#endif // !INTERPRETER_H
