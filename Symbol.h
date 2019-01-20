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

	//constructor for a variable
	Symbol(const int val);

	//constructor for a user-defined function symbol
	Symbol(list<Token> functionDef);

	//constructor for a keyword/operator
	Symbol(functionPointer fptr);

	//Generic contructor
	Symbol(Types type, const int val, list<Token> functionDef, functionPointer fptr);

	Types getType();

	int getValue();

private:
	//Determines if this symbol is a keyword/operator or a variable.
	Types type;

	//The value stored in this variable
	int value;

	//Pointer to a list of tokens encountered inside a function definition
	list<Token> functionDef;

	//function pointer to the function executed when this symbol is read
	functionPointer function;

	friend Interpreter;
};

#endif // !SYMBOL_H
