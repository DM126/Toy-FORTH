#ifndef SYMBOL_H
#define SYMBOL_H

#include <list>
#include "Token.h"

//Function pointer used by keyword/operator symbols
class Interpreter;
using functionPointer = void (*)(Interpreter *iptr);

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

	Symbol(const Symbol& other);
	Symbol& operator=(const Symbol& rhs);

	//constructor for a variable
	explicit Symbol(const int val);

	//constructor for a user-defined function symbol
	explicit Symbol(std::list<Token> const & functionDef);

	//constructor for a keyword/operator
	explicit Symbol(functionPointer fptr);

	//constructor for an array symbol
	Symbol(int* arr, const int size);

	//Generic contructor
	Symbol(Types type, const int val, std::list<Token> const & functionDef, functionPointer fptr, int* arr);

	Types getType() const;

	int getValue() const;

	//Reallocates space for an array after if ALLOT is called again. Keeps items
	//already in the array (as long as they don't get truncated with a smaller
	//size).
	void reallocate(const int newSize);

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
