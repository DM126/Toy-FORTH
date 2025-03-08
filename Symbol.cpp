#include <list>
#include <algorithm>
#include "Symbol.h"

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

Symbol::Symbol(std::list<Token> const & functionDef)
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

	//Initialize all array elements to 0.
	for (int i = 0; i < size; i++)
	{
		array[i] = 0;
	}
}

Symbol::Symbol(Types type, const int val, std::list<Token> const & functionDef, functionPointer fptr, int* arr)
{
	this->type = type;
	value = val;
	this->functionDef = functionDef;
	function = fptr;
	array = arr;
}

Types Symbol::getType() const
{
	return type;
}

int Symbol::getValue() const
{
	return value;
}

void Symbol::reallocate(const int newSize)
{
	if (array != nullptr)
	{
		auto newArray = new int[newSize];
		int smallerSize = std::min(value, newSize);
		for (int i = 0; i < smallerSize; i++)
		{
			newArray[i] = array[i];
		}
		delete[] array;
		array = newArray;
		value = newSize;
	}
}
