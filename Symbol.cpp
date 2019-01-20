#include <list>
#include "Symbol.h"

using namespace std;

Symbol::Symbol()
{
	type = UNKNOWN;
	value = 0;
	function = nullptr;
}

Symbol::Symbol(const int val)
{
	type = VARIABLE;
	value = val;
	function = nullptr;
}

Symbol::Symbol(list<Token> functionDef)
{
	type = FUNCTION;
	value = 0;
	this->functionDef = functionDef;
	function = nullptr;
}

Symbol::Symbol(functionPointer fptr)
{
	type = KEYWORD;
	value = 0;
	function = fptr;
}

Symbol::Symbol(Types type, const int val, list<Token> functionDef, functionPointer fptr)
{
	this->type = type;
	value = val;
	this->functionDef = functionDef;
	function = fptr;
}

Types Symbol::getType()
{
	return type;
}

int Symbol::getValue()
{
	return value;
}
