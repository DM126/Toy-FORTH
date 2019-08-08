#include <list>
#include "Symbol.h"

using namespace std;

Symbol::Symbol()
{
	type = UNKNOWN;
	value = 0;
	function = nullptr;
	array = nullptr;
}

Symbol::~Symbol()
{
	if (array != nullptr)
	{
		delete[] array;
	}
}

Symbol::Symbol(const int val)
{
	type = VARIABLE;
	value = val;
	function = nullptr;
	array = nullptr;
}

Symbol::Symbol(list<Token> functionDef)
{
	type = FUNCTION;
	value = 0;
	this->functionDef = functionDef;
	function = nullptr;
	array = nullptr;
}

Symbol::Symbol(functionPointer fptr)
{
	type = KEYWORD;
	value = 0;
	function = fptr;
	array = nullptr;
}

Symbol::Symbol(int * arr, const int size)
{
	type = ARRAY;
	value = size;
	function = nullptr;
	array = arr;
}

Symbol::Symbol(Types type, const int val, list<Token> functionDef, functionPointer fptr, int* arr)
{
	this->type = type;
	value = val;
	this->functionDef = functionDef;
	function = fptr;
	array = arr;
}

Types Symbol::getType()
{
	return type;
}

int Symbol::getValue()
{
	return value;
}
