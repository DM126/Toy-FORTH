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
		array = nullptr;
	}
}

Symbol::Symbol(const Symbol& other)
{
	type = other.type;
	value = other.value;
	function = other.function;
	functionDef = other.functionDef; //TODO Will this work?
	if (other.type == ARRAY)
	{
		array = new int[other.value];
		for (int i = 0; i < other.value; i++)
		{
			array[i] = other.array[i];
		}
	}
	else
	{
		array = nullptr;
	}
}

Symbol & Symbol::operator=(const Symbol& rhs)
{
	if (this != &rhs)
	{
		if (array != nullptr)
		{
			delete[] array;
			array = nullptr;
		}

		type = rhs.type;
		value = rhs.value;
		function = rhs.function;
		functionDef = rhs.functionDef; //TODO Will this work?
		if (rhs.type == ARRAY)
		{
			array = new int[rhs.value];
			for (int i = 0; i < rhs.value; i++)
			{
				array[i] = rhs.array[i];
			}
		}
		else
		{
			array = nullptr;
		}
	}

	return *this;
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
