#ifndef SYMBOL_H
#define SYMBOL_H

#include <list>
#include "Token.h"

//Function pointer used by keyword/operator symbols
class Interpreter;
typedef void(*functionPointer)(Interpreter *iptr);

//Symbols represent keywords, operators, and variables.
//Every time a symbol is encountered in the program, it is checked in a map
//of all the symbols. Keywords and operators have function pointers that are
//executed whenever they are read. Variable symbols store an integer value.
class Symbol
{
public:
	Symbol();

	//Destructor for freeing arrays if allocated
	~Symbol();

	//TODO: copy constructor and overloaded assignment operator are unnecessary,
	//		but could be implemented for good practice...

	//constructor for a variable
	Symbol(const int val);

	//constructor for a user-defined function symbol
	Symbol(std::list<Token> functionDef);

	//constructor for a keyword/operator
	Symbol(functionPointer fptr);

	//constructor for an array symbol
	Symbol(int* arr, const int size);

	//Generic contructor
	Symbol(Types type, const int val, std::list<Token> functionDef, functionPointer fptr, int* arr);

	Types getType();

	int getValue();

private:
	//Determines if this symbol is a keyword/operator, variable, or an array.
	Types type;

	//The value stored in this variable. For arrays, this represents the size of the array.
	int value;

	//Pointer to a list of tokens encountered inside a function definition
	std::list<Token> functionDef;

	//function pointer to the function executed when this symbol is read
	functionPointer function;

	//Pointer to an array
	int* array;

	friend Interpreter;
};

#endif // !SYMBOL_H
