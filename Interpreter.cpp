#include <iostream>
#include <time.h>
#include "Interpreter.h"
#include "Parser.h"

//Sets up the symbol table and begins the parser
Interpreter::Interpreter(const std::string& fileName)
{
	//See Interpreter.h for information on each symbol and their functions.
	symTab["DUMP"] = Symbol(&doDUMP);

	symTab["+"] = Symbol(&doPlus);
	symTab["-"] = Symbol(&doMinus);
	symTab["*"] = Symbol(&doMultiply);
	symTab["/"] = Symbol(&doDivide);
	symTab["%"] = Symbol(&doMod);
	symTab["NEG"] = Symbol(&doNEG);

	symTab["."] = Symbol(&doDOT);
	symTab["CR"] = Symbol(&doCR);
	symTab["SP"] = Symbol(&doSP);
	symTab["EMIT"] = Symbol(&doEMIT);

	symTab["DUP"] = Symbol(&doDUP);
	symTab["DROP"] = Symbol(&doDROP);
	symTab["SWAP"] = Symbol(&doSWAP);
	symTab["ROT"] = Symbol(&doROT);

	symTab["SET"] = Symbol(&doSET);
	symTab["@"] = Symbol(&doAt);
	symTab["!"] = Symbol(&doStore);
	symTab["ALLOT"] = Symbol(&doALLOT);
	symTab["#@"] = Symbol(&doArrayAt);
	symTab["#!"] = Symbol(&doArrayStore);

	symTab["<"] = Symbol(&doLessThan);
	symTab["<="] = Symbol(&doLessThanOrEqual);
	symTab["=="] = Symbol(&doEqualTo);
	symTab["!="] = Symbol(&doNotEqual);
	symTab[">="] = Symbol(&doGreaterThanOrEqual);
	symTab[">"] = Symbol(&doGreaterThan);

	symTab["AND"] = Symbol(&doAND);
	symTab["OR"] = Symbol(&doOR);
	symTab["NOT"] = Symbol(&doNOT);

	symTab["IFTHEN"] = Symbol(&doIFTHEN);
	symTab["ELSE"] = Symbol(nullptr);
	symTab["ENDIF"] = Symbol(nullptr);

	symTab["DO"] = Symbol(&doDO);
	symTab["UNTIL"] = Symbol(&doUNTIL);

	symTab["DEFINE"] = Symbol(&doDEFINE);
	symTab["END"] = Symbol(nullptr);

	symTab["RANDOM"] = Symbol(&doRANDOM);
	srand((unsigned int)time(nullptr));

	symTab["INPUT"] = Symbol(&doINPUT);

	symTab["OPEN"] = Symbol(&doOPEN);
	symTab["READ"] = Symbol(&doREAD);
	
	try
	{
		Parser parser(fileName, symTab); //TODO look for file first?
		tokenBuffer = parser.getTokens();
		//printTokenBuffer();

		mainLoop();
	}
	catch (std::runtime_error& e) //If file name is incorrect (TODO: Custom exception?)
	{
		std::cerr << e.what() << std::endl;
	}
}

void Interpreter::mainLoop()
{
	Token next;

	try
	{
		while (!tokenBuffer.empty())
		{
			next = nextToken();

			if (next.type == Types::KEYWORD)
			{
				//TODO: WHAT ABOUT VARIABLES?
				if (symTab[next.text].function != nullptr)
				{
					symTab[next.text].function(this);
				}
			}
			else if (symTab.find(next.text) != symTab.end() && symTab[next.text].type == Types::FUNCTION)
			{
				//Add all the tokens from the function definition to the token buffer
				std::list<Token> copy = symTab[next.text].functionDef;
				tokenBuffer.splice(tokenBuffer.begin(), copy);
			}
			else
			{
				params.push(next);
			}
		}
	}
	catch (std::underflow_error& e)
	{
		std::cout << "Parameter stack underflow." << std::endl;
		std::cout << e.what() << std::endl;
	}
	catch (std::invalid_argument& e)
	{
		std::cout << e.what() << std::endl;
	}

	printEndInfo();

	//deleteFunctions();
}

Token Interpreter::nextToken()
{
	Token t = tokenBuffer.front();
	tokenBuffer.pop_front();

	return t;
}

//void Interpreter::deleteFunctions()
//{
//	map<std::string, Symbol>::iterator symbol = symTab.begin();
//	while (symbol != symTab.end())
//	{
//		if (symbol->second.type == FUNCTION)
//		{
//			delete symbol->second.functionDef;
//		}
//		itr = itr.next();
//	}
//}

Token Interpreter::popStack()
{
	Token t = params.top();
	params.pop();
	//TODO: CHECK FOR ISEMPTY?
	return t;
}

bool Interpreter::isSymbol(const Token& t)
{
	return symTab.find(t.text) != symTab.end();
}

void Interpreter::checkStackSize(const unsigned int minItems, const std::string & operationName) const
{
	if (params.size() < minItems)
	{
		std::string message = "Error: Stack needs " + std::to_string(minItems) + " items for " + operationName + ".";
		throw std::underflow_error(message);
	}
}

void Interpreter::printSymTab()
{
	auto itr = symTab.begin();
	while (itr != symTab.end())
	{
		std::cout << itr->first << " - " << typeArray[static_cast<int>(itr->second.type)] << std::endl;
		itr++;
	}
}

void Interpreter::printTokenBuffer()
{
	auto itr = tokenBuffer.begin();
	while (itr != tokenBuffer.end())
	{
		std::cout << *itr << std::endl;
		itr++;
	}
}

void Interpreter::printEndInfo() const
{
	std::cout << "\n----------------------------------------\n"
		<< "End of program\n";
	if (params.empty())
	{
		std::cout << "Stack parameter empty.\n";
	}
	else
	{
		std::cout << params.size() << " item(s) left on the stack.\n";
	}
}




void Interpreter::doDUMP(Interpreter *iptr)
{
	std::stack<Token> copy = iptr->params;
	std::cout << "DUMP:\n-----------------------------------\n";
	while (!copy.empty())
	{
		std::cout << copy.top() << std::endl;
		copy.pop();
	}
	std::cout << "END DUMP\n-----------------------------------\n";
}




//Arithmetic operations

void Interpreter::doPlus(Interpreter *iptr)
{
	iptr->checkStackSize(2, "+");

	Token t1 = iptr->popStack();
	Token t2 = iptr->popStack();
	
	int results = t2.value + t1.value;
	iptr->params.emplace(results);
}

void Interpreter::doMinus(Interpreter *iptr)
{
	iptr->checkStackSize(2, "-");

	Token t1 = iptr->popStack();
	Token t2 = iptr->popStack();

	int results = t2.value - t1.value;
	iptr->params.emplace(results);
}

void Interpreter::doMultiply(Interpreter *iptr)
{
	iptr->checkStackSize(2, "*");

	Token t1 = iptr->popStack();
	Token t2 = iptr->popStack();

	int results = t2.value * t1.value;
	iptr->params.emplace(results);
}

void Interpreter::doDivide(Interpreter *iptr)
{
	iptr->checkStackSize(2, "/");

	Token t1 = iptr->popStack();
	Token t2 = iptr->popStack();

	int results = t2.value / t1.value; //TODO: DIV BY ZERO?
	iptr->params.emplace(results);
}

void Interpreter::doMod(Interpreter * iptr)
{
	iptr->checkStackSize(2, "%");

	Token t1 = iptr->popStack();
	Token t2 = iptr->popStack();

	int results = t2.value % t1.value; //TODO: DIV BY ZERO?
	iptr->params.emplace(results);
}

void Interpreter::doNEG(Interpreter * iptr)
{
	iptr->checkStackSize(1, "NEG");

	Token t = iptr->popStack();

	//TODO: I FEEL LIKE CREATING NEW TOKENS IS WASTEFUL WHEN YOU COULD
	//JUST EDIT THE TOKEN ON THE TOP, THEY ARE FRIENDS AFTER ALL.... IDK,
	//MAYBE KEEPING THESE FUNCTIONS CONSISTENT IS A GOOD IDEA? IDK... :/
	//AND ANOTHER THING: I FEEL LIKE YOU DON'T NEED TO COPY THE POPPED TOKENS,
	//JUST STORE THE VALUES YOU NEED AND WHO CARES ABOUT THE REST!!!
	iptr->params.emplace(-t.value);
}





//Printing operations

void Interpreter::doDOT(Interpreter *iptr)
{
	iptr->checkStackSize(1, ".");

	Token t = iptr->popStack();

	if (t.type == Types::INTEGER)
	{
		std::cout << t.value;
	}
	else
	{
		std::cout << t.text;
	}
}

void Interpreter::doCR(Interpreter*)
{
	std::cout << "\n";
}

void Interpreter::doSP(Interpreter*)
{
	std::cout << " ";
}

void Interpreter::doEMIT(Interpreter * iptr)
{
	iptr->checkStackSize(1, "EMIT");

	Token t = iptr->popStack();

	//TODO: Only allow integer types for EMIT?
	auto ch = static_cast<char>(t.value);
	std::cout << ch;
}





//Stack operations

void Interpreter::doDUP(Interpreter *iptr)
{
	iptr->checkStackSize(1, "DUP");

	Token t = iptr->params.top();
	iptr->params.push(t);
}

void Interpreter::doDROP(Interpreter *iptr)
{
	iptr->checkStackSize(1, "DROP");

	iptr->popStack();
}

void Interpreter::doSWAP(Interpreter *iptr)
{
	iptr->checkStackSize(2, "SWAP");

	Token t1 = iptr->popStack();
	Token t2 = iptr->popStack();

	iptr->params.push(t1);
	iptr->params.push(t2);
}

void Interpreter::doROT(Interpreter *iptr)
{
	iptr->checkStackSize(3, "ROT");

	Token t1 = iptr->popStack();
	Token t2 = iptr->popStack();
	Token t3 = iptr->popStack();

	iptr->params.push(t2);
	iptr->params.push(t1);
	iptr->params.push(t3);
}






//Variable operations

//Validates that a variable operation is using a valid identifier.
void validateIdentifier(Token const & token)
{
	//TODO: Create a custom exception type for this situation and catch that in the mainloop
	if (token.getType() == Types::INTEGER)
	{
		throw std::invalid_argument("Error: cannot use an integer (" + std::to_string(token.getValue()) + ") as an identifier.");
	}
}

//TODO: HOW DO THE PARSER AND SYMTAB DIFFERENTIATE BETWEEN SYMBOLS AND VARIABLES?
//ALSO, WHAT IS THE PURPOSE OF THE VARIABLE ENUM TYPE?
void Interpreter::doSET(Interpreter *iptr)
{
	iptr->checkStackSize(2, "SET");

	Token varName = iptr->popStack();
	validateIdentifier(varName);
	
	Token val = iptr->popStack();

	//If the variable does not already exist, enter it in the symbol table.
	if (!iptr->isSymbol(varName))
	{
		iptr->symTab[varName.text] = Symbol(val.value);
	}
	else
	{
		std::cerr << "\nError: Cannot perform 'SET': The variable \"" << varName.text << "\" already exists.\n";
	}
}

void Interpreter::doAt(Interpreter *iptr)
{
	iptr->checkStackSize(1, "@");

	Token var = iptr->popStack();
	validateIdentifier(var);

	//If the variable exists, place its value from the symbol table onto the stack.
	if (iptr->isSymbol(var))
	{
		iptr->params.emplace(iptr->symTab[var.text].value);
	}
	else
	{
		std::cerr << "\nError: Cannot perform '@': The variable \"" << var.text << "\" does not exist.\n";
	}
}

void Interpreter::doStore(Interpreter *iptr)
{
	iptr->checkStackSize(2, "!");

	Token varName = iptr->popStack();
	validateIdentifier(varName);

	Token val = iptr->popStack();

	//If the variable exists, store its value in its symbol table entry.
	if (iptr->isSymbol(varName))
	{
		iptr->symTab[varName.text].value = val.value;
	}
	else
	{
		std::cerr << "\nError: Cannot perform '!': The variable \"" << varName.text << "\" does not exist.\n";
	}
}

void Interpreter::doALLOT(Interpreter *iptr)
{
	iptr->checkStackSize(2, "ALLOT");

	//format: size arrayName ALLOT

	Token arrayName = iptr->popStack();
	validateIdentifier(arrayName);

	Token size = iptr->popStack();

	//Validate array size is positive
	if (size.value <= 0)
	{
		throw std::invalid_argument("Arrays must have a positive size.");
	}

	//If the array does not exist, create an array with that size.
	if (!iptr->isSymbol(arrayName))
	{
		auto arr = new int[size.value];
		iptr->symTab[arrayName.text] = Symbol(arr, size.value);
	}
	else
	{
		iptr->symTab[arrayName.text].reallocate(size.value);
	}
}

void Interpreter::doArrayAt(Interpreter *iptr)
{
	iptr->checkStackSize(2, "#@");

	//array index #@
	Token index = iptr->popStack();
	Token array = iptr->popStack();
	validateIdentifier(array);

	//If the array exists and the index is valid, place its value from the
	//symbol table onto the stack.
	if (iptr->isSymbol(array))
	{
		if (index.value >= 0 && index.value < iptr->symTab[array.text].value)
		{
			iptr->params.emplace(iptr->symTab[array.text].array[index.value]);
		}
		else
		{
			std::cerr << "\nError: Array index out of bounds: " << index.value << ".\n";
		}
	}
	else
	{
		std::cerr << "\nError: The array \"" << array.text << "\" does not exist.\n";
	}
}

void Interpreter::doArrayStore(Interpreter *iptr)
{
	iptr->checkStackSize(3, "#!");

	//val array index #!
	Token index = iptr->popStack();
	Token array = iptr->popStack();
	Token val = iptr->popStack();
	validateIdentifier(array);

	//If the array exists and the index is valid, enter the value into the index
	if (iptr->isSymbol(array))
	{
		if (index.value >= 0 && index.value < iptr->symTab[array.text].value)
		{
			iptr->symTab[array.text].array[index.value] = val.value;
		}
		else
		{
			std::cerr << "\nError: Array index out of bounds: " << index.value << ".\n";
		}
	}
	else
	{
		std::cerr << "\nError: The array \"" << array.text << "\" does not exist.\n";
	}
}






//Comparison operations

void Interpreter::doLessThan(Interpreter *iptr)
{
	iptr->checkStackSize(2, "<");

	//left right <
	Token right = iptr->popStack();
	Token left = iptr->popStack();

	bool b = (left.value < right.value);
	iptr->params.emplace(b);
}

void Interpreter::doLessThanOrEqual(Interpreter *iptr)
{
	iptr->checkStackSize(2, "<=");

	//left right <=
	Token right = iptr->popStack();
	Token left = iptr->popStack();

	bool b = (left.value <= right.value);
	iptr->params.emplace(b);
}

void Interpreter::doEqualTo(Interpreter *iptr)
{
	iptr->checkStackSize(2, "==");

	//left right ==
	Token right = iptr->popStack();
	Token left = iptr->popStack();

	bool b = (left.value == right.value);
	iptr->params.emplace(b);
}

void Interpreter::doNotEqual(Interpreter *iptr)
{
	iptr->checkStackSize(2, "!=");

	//left right !=
	Token right = iptr->popStack();
	Token left = iptr->popStack();

	bool b = (left.value != right.value);
	iptr->params.emplace(b);
}

void Interpreter::doGreaterThanOrEqual(Interpreter *iptr)
{
	iptr->checkStackSize(2, ">=");

	//left right >=
	Token right = iptr->popStack();
	Token left = iptr->popStack();

	bool b = (left.value >= right.value);
	iptr->params.emplace(b);
}

void Interpreter::doGreaterThan(Interpreter *iptr)
{
	iptr->checkStackSize(2, ">");

	//left right >
	Token right = iptr->popStack();
	Token left = iptr->popStack();

	bool b = (left.value > right.value);
	iptr->params.emplace(b);
}






//Logic operations

void Interpreter::doAND(Interpreter *iptr)
{
	iptr->checkStackSize(2, "AND");

	Token t1 = iptr->popStack();
	Token t2 = iptr->popStack();

	bool b = (t1.value && t2.value);
	iptr->params.emplace(b);
}

void Interpreter::doOR(Interpreter *iptr)
{
	iptr->checkStackSize(2, "OR");

	Token t1 = iptr->popStack();
	Token t2 = iptr->popStack();

	bool b = (t1.value || t2.value);
	iptr->params.emplace(b);
}

void Interpreter::doNOT(Interpreter *iptr)
{
	iptr->checkStackSize(1, "NOT");

	Token t = iptr->popStack();

	iptr->params.emplace(!t.value);
}






//Conditional operations

void Interpreter::doIFTHEN(Interpreter *iptr)
{
	iptr->checkStackSize(1, "IFTHEN");

	Token p = iptr->popStack();

	if (p.value)
	{
		std::list<Token> ifBuffer;
		addToIfBuffer(iptr, ifBuffer);

		//Add everything found between IFTHEN and ELSE to tokenBuffer
		iptr->tokenBuffer.splice(iptr->tokenBuffer.begin(), ifBuffer);
	}
	else //if the value was false, skip over the tokens inside the if part.
	{
		skipBlock(iptr, true);
	}
}

void Interpreter::addToIfBuffer(Interpreter *iptr, std::list<Token> & ifBuffer)
{
	bool foundEnd = false;
	int ifThens = 0;
	int endIfs = 0;
	Token next;

	do
	{
		next = iptr->nextToken();

		if (iptr->isSymbol(next))
		{
			if (next.text == "ELSE" && ifThens <= endIfs)
			{
				//Skip everything between the else and endif
				skipBlock(iptr, false);
				foundEnd = true;
			}
			else if (next.text == "IFTHEN")
			{
				ifThens++;
				ifBuffer.push_back(next);
			}
			else if (next.text == "ENDIF")
			{
				endIfs++;
				ifBuffer.push_back(next);

				//no else statement found, end the loop
				foundEnd = endIfs > ifThens;
			}
			else
			{
				ifBuffer.push_back(next);
			}
		}
		else
		{
			ifBuffer.push_back(next);
		}
	}
	while (!foundEnd);
}

void Interpreter::skipBlock(Interpreter *iptr, bool skipToElse)
{
	int ifThens = 0;
	int endIfs = 0;
	Token next;
	bool foundEnd = false;

	//count the number of IFTHEN and ENDIF tokens before the ELSE to skip
	//any nested if else statements.
	do
	{
		next = iptr->nextToken();

		//if the next token is a keyword, check if it's IFTHEN, ENDIF, or ELSE.
		if (iptr->isSymbol(next))
		{
			if (next.text == "IFTHEN")
			{
				ifThens++;
			}
			else if (next.text == "ENDIF")
			{
				endIfs++;
				//if more endifs than ifthens then no else statement was found, end the loop
				foundEnd = endIfs > ifThens;
			}
			else if (skipToElse && next.text == "ELSE" && ifThens <= endIfs)
			{
				foundEnd = true;
			}
		}
	}
	while (!foundEnd);
}




//Loop operations

void Interpreter::doDO(Interpreter *iptr)
{
	std::list<Token> buffer;

	Token next;

	//Each DO should have 1 UNTIL
	int dos = 1;
	int untils = 0;

	//Save the tokens between DO and UNTIL in a list
	do
	{
		next = iptr->nextToken();
		buffer.push_back(next);
		if (iptr->isSymbol(next))
		{
			if (next.text == "DO")
			{
				dos++;
			}
			else if (next.text == "UNTIL")
			{
				untils++;
			}
		}
	}//end the loop once the number of DOs and UNTILs are matched.
	while (dos > untils);

	//Add the saved tokens back to the token buffer and push the created list 
	//onto the stack of lists
	std::list<Token> copy = buffer;
	iptr->loopBuffers.push(buffer);
	iptr->tokenBuffer.splice(iptr->tokenBuffer.begin(), copy);
}

void Interpreter::doUNTIL(Interpreter *iptr)
{
	iptr->checkStackSize(1, "UNTIL");

	Token p = iptr->popStack();

	//Return the tokens to the token buffer if the loop is still going,
	//otherwise empty the list.
	if (!p.value)
	{
		std::list<Token> copy = iptr->loopBuffers.top();
		iptr->tokenBuffer.splice(iptr->tokenBuffer.begin(), copy);
	}
	else
	{
		iptr->loopBuffers.pop();
	}
}




//Function operation

void Interpreter::doDEFINE(Interpreter *iptr)
{
	iptr->checkStackSize(1, "DEFINE");

	Token function = iptr->popStack();

	if (iptr->isSymbol(function))
	{
		std::cerr << "Error: The identifier \"" << function.text << "\" already exists." << std::endl;
		return; //TODO: THROW AN EXCEPTION HERE?
	}

	std::list<Token> tokens;

	//get all tokens until END is encountered
	Token next = iptr->nextToken();
	while (!iptr->isSymbol(next) || next.text != "END")
	{
		tokens.push_back(next);
		next = iptr->nextToken();
	}

	//Add the function to the symbol table
	iptr->symTab[function.text] = Symbol(tokens);
}







void Interpreter::doRANDOM(Interpreter * iptr)
{
	iptr->params.emplace(rand());
}





void Interpreter::doINPUT(Interpreter * iptr)
{
	//TODO: WHAT ABOUT STRINGS????
	int input;

	std::cin >> input;

	iptr->params.emplace(input);
}






void Interpreter::doOPEN(Interpreter * iptr)
{
	iptr->checkStackSize(1, "OPEN");

	Token filename = iptr->popStack();

	//TODO: Should these errors stop execution?
	if (filename.type == Types::LITERAL)
	{
		iptr->file = std::ifstream(filename.text);
		if (iptr->file.fail())
		{
			std::cerr << "Error: Could not open file: " << filename.text << "\n";
		}
	}
	else
	{
		std::cerr << "ERROR: File name must be a string.\n";
	}
}

void Interpreter::doREAD(Interpreter * iptr)
{
	//TODO: Should failing this stop execution?
	if (iptr->file.is_open())
	{
		//TODO: Check for EOF
		iptr->params.emplace(iptr->file.get());
	}
	else
	{
		std::cerr << "Error: no file is currently open.\n";
	}
}
