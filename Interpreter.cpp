#include <iostream>
#include <time.h>
#include "Interpreter.h"
#include "Parser.h"

using namespace std;

//Sets up the symbol table and begins the parser
Interpreter::Interpreter(const string& fileName)
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

	symTab["DUP"] = Symbol(&doDUP);
	symTab["DROP"] = Symbol(&doDROP);
	symTab["SWAP"] = Symbol(&doSWAP);
	symTab["ROT"] = Symbol(&doROT);

	symTab["SET"] = Symbol(&doSET);
	symTab["@"] = Symbol(&doAt);
	symTab["!"] = Symbol(&doStore);

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
	srand((unsigned int)time(0));

	symTab["INPUT"] = Symbol(&doINPUT);

	Parser parser(fileName, symTab);
	tkBuffer = parser.getTokens();
	//printTokenBuffer();

	mainLoop();
}

void Interpreter::mainLoop()
{
	Token next;

	try
	{
		while (!tkBuffer.empty())
		{
			next = nextToken();

			if (next.type == KEYWORD)
			{
				//TODO: WHAT ABOUT VARIABLES?
				if (symTab[next.text].function != nullptr)
				{
					symTab[next.text].function(this);
				}
			}
			else if (symTab.find(next.text) != symTab.end() && symTab[next.text].type == FUNCTION)
			{
				//Add all the tokens from the function definition to the token buffer
				list<Token> copy = symTab[next.text].functionDef;
				tkBuffer.splice(tkBuffer.begin(), copy);
			}
			else
			{
				params.push(next);
			}
		}
	}
	catch (underflow_error& e)
	{
		cout << "Parameter stack underflow." << endl;
		cout << e.what() << endl;
	}
	catch (invalid_argument& e)
	{
		cout << e.what() << endl;
	}

	printEndInfo();

	//deleteFunctions();
}

Token Interpreter::nextToken()
{
	Token t = tkBuffer.front();
	tkBuffer.pop_front();

	return t;
}

//void Interpreter::deleteFunctions()
//{
//	map<string, Symbol>::iterator symbol = symTab.begin();
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

void Interpreter::checkStackSize(const unsigned int minItems, const string & operationName)
{
	if (params.size() < minItems)
	{
		string message = "Error: Stack needs " + to_string(minItems) + " items for " + operationName + ".";
		throw underflow_error(message);
	}
}

void Interpreter::printSymTab()
{
	map<string, Symbol>::iterator itr = symTab.begin();
	while (itr != symTab.end())
	{
		cout << itr->first << " - " << typeArray[itr->second.type] << endl;
		itr++;
	}
}

void Interpreter::printTokenBuffer()
{
	list<Token>::iterator itr = tkBuffer.begin();
	while (itr != tkBuffer.end())
	{
		cout << *itr << endl;
		itr++;
	}
}

void Interpreter::printEndInfo()
{
	cout << "\n----------------------------------------\n"
		<< "End of program\n";
	if (params.empty())
	{
		cout << "Stack parameter empty.\n";
	}
	else
	{
		cout << params.size() << " item(s) left on the stack.\n";
	}
}




void Interpreter::doDUMP(Interpreter *iptr)
{
	stack<Token> copy = iptr->params;
	cout << "DUMP:\n-----------------------------------\n";
	while (!copy.empty())
	{
		cout << copy.top() << endl;
		copy.pop();
	}
	cout << "END DUMP\n-----------------------------------\n";
}




//Arithmetic operations

void Interpreter::doPlus(Interpreter *iptr)
{
	iptr->checkStackSize(2, "+");

	Token t1 = iptr->popStack();
	Token t2 = iptr->popStack();
	
	int results = t2.value + t1.value;
	iptr->params.push(Token(INTEGER, "", results));
}

void Interpreter::doMinus(Interpreter *iptr)
{
	iptr->checkStackSize(2, "-");

	Token t1 = iptr->popStack();
	Token t2 = iptr->popStack();

	int results = t2.value - t1.value;
	iptr->params.push(Token(INTEGER, "", results));
}

void Interpreter::doMultiply(Interpreter *iptr)
{
	iptr->checkStackSize(2, "*");

	Token t1 = iptr->popStack();
	Token t2 = iptr->popStack();

	int results = t2.value * t1.value;
	iptr->params.push(Token(INTEGER, "", results));
}

void Interpreter::doDivide(Interpreter *iptr)
{
	iptr->checkStackSize(2, "/");

	Token t1 = iptr->popStack();
	Token t2 = iptr->popStack();

	int results = t2.value / t1.value; //TODO: DIV BY ZERO?
	iptr->params.push(Token(INTEGER, "", results));
}

void Interpreter::doMod(Interpreter * iptr)
{
	iptr->checkStackSize(2, "%");

	Token t1 = iptr->popStack();
	Token t2 = iptr->popStack();

	int results = t2.value % t1.value; //TODO: DIV BY ZERO?
	iptr->params.push(Token(INTEGER, "", results));
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
	iptr->params.push(Token(INTEGER, "", -t.value));
}





//Printing operations

void Interpreter::doDOT(Interpreter *iptr)
{
	iptr->checkStackSize(1, ".");

	Token t = iptr->popStack();

	if (t.type == INTEGER)
	{
		cout << t.value;
	}
	else
	{
		cout << t.text;
	}
}

void Interpreter::doCR(Interpreter *iptr)
{
	cout << "\n";
}

void Interpreter::doSP(Interpreter *iptr)
{
	cout << " ";
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

//TODO: HOW DO THE PARSER AND SYMTAB DIFFERENTIATE BETWEEN SYMBOLS AND VARIABLES?
//ALSO, WHAT IS THE PURPOSE OF THE VARIABLE ENUM TYPE?
void Interpreter::doSET(Interpreter *iptr)
{
	iptr->checkStackSize(2, "SET");

	Token varName = iptr->popStack();

	//TODO: Create a custom exception type for this situation and catch that in the mainloop
	//TODO: ADD THIS FOR ALL VARIABLE OPERATIONS?
	if (varName.type == INTEGER)
	{
		throw invalid_argument("Cannot use an integer as an identifier.");
	}
	
	Token val = iptr->popStack();

	//If the variable does not already exist, enter it in the symbol table.
	if (!iptr->isSymbol(varName))
	{
		iptr->symTab[varName.text] = Symbol(val.value);
	}
	else
	{
		cerr << "\nError: The variable \"" << varName.text << "\" already exists.\n";
	}
}

void Interpreter::doAt(Interpreter *iptr)
{
	iptr->checkStackSize(1, "@");

	Token var = iptr->popStack();

	//If the variable exists, place its value from the symbol table onto the stack.
	if (iptr->isSymbol(var))
	{
		iptr->params.push(Token(INTEGER, "", iptr->symTab[var.text].value));
	}
	else
	{
		cerr << "\nError: The variable \"" << var.text << "\" does not exist.\n";
	}
}

void Interpreter::doStore(Interpreter *iptr)
{
	iptr->checkStackSize(2, "!");

	Token varName = iptr->popStack();
	Token val = iptr->popStack();

	//If the variable exists, store its value in its symbol table entry.
	if (iptr->isSymbol(varName))
	{
		iptr->symTab[varName.text].value = val.value;
	}
	else
	{
		cerr << "\nError: The variable \"" << varName.text << "\" does not exist.\n";
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
	iptr->params.push(Token(INTEGER, "", b));
}

void Interpreter::doLessThanOrEqual(Interpreter *iptr)
{
	iptr->checkStackSize(2, "<=");

	//left right <=
	Token right = iptr->popStack();
	Token left = iptr->popStack();

	bool b = (left.value <= right.value);
	iptr->params.push(Token(INTEGER, "", b));
}

void Interpreter::doEqualTo(Interpreter *iptr)
{
	iptr->checkStackSize(2, "==");

	//left right ==
	Token right = iptr->popStack();
	Token left = iptr->popStack();

	bool b = (left.value == right.value);
	iptr->params.push(Token(INTEGER, "", b));
}

void Interpreter::doNotEqual(Interpreter *iptr)
{
	iptr->checkStackSize(2, "!=");

	//left right !=
	Token right = iptr->popStack();
	Token left = iptr->popStack();

	bool b = (left.value != right.value);
	iptr->params.push(Token(INTEGER, "", b));
}

void Interpreter::doGreaterThanOrEqual(Interpreter *iptr)
{
	iptr->checkStackSize(2, ">=");

	//left right >=
	Token right = iptr->popStack();
	Token left = iptr->popStack();

	bool b = (left.value >= right.value);
	iptr->params.push(Token(INTEGER, "", b));
}

void Interpreter::doGreaterThan(Interpreter *iptr)
{
	iptr->checkStackSize(2, ">");

	//left right >
	Token right = iptr->popStack();
	Token left = iptr->popStack();

	bool b = (left.value > right.value);
	iptr->params.push(Token(INTEGER, "", b));
}






//Logic operations

void Interpreter::doAND(Interpreter *iptr)
{
	iptr->checkStackSize(2, "AND");

	Token t1 = iptr->popStack();
	Token t2 = iptr->popStack();

	bool b = (t1.value && t2.value);
	iptr->params.push(Token(INTEGER, "", b));
}

void Interpreter::doOR(Interpreter *iptr)
{
	iptr->checkStackSize(2, "OR");

	Token t1 = iptr->popStack();
	Token t2 = iptr->popStack();

	bool b = (t1.value || t2.value);
	iptr->params.push(Token(INTEGER, "", b));
}

void Interpreter::doNOT(Interpreter *iptr)
{
	iptr->checkStackSize(1, "NOT");

	Token t = iptr->popStack();

	iptr->params.push(Token(INTEGER, "", !t.value));
}






//Conditional operations

void Interpreter::doIFTHEN(Interpreter *iptr)
{
	iptr->checkStackSize(1, "IFTHEN");

	Token p = iptr->popStack();

	int ifThens = 0, endIfs = 0;
	Token next;

	if (p.value)
	{
		bool foundEnd = false;
		list<Token> ifBuffer;

		do
		{
			next = iptr->nextToken();

			if (iptr->isSymbol(next))
			{
				if (next.text == "ELSE" && ifThens <= endIfs)
				{
					ifThens = 0;
					endIfs = 0;

					//Skip everything between the else and endif
					do
					{
						next = iptr->nextToken();
						if (iptr->isSymbol(next))
						{
							if (next.text == "IFTHEN")
							{
								ifThens++;
							}
							else if (next.text == "ENDIF")
							{
								endIfs++;

								if (endIfs > ifThens)
								{
									foundEnd = true;
								}
							}
						}
					}
					while (!foundEnd);
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

					if (endIfs > ifThens)
					{
						//no else statement found, end the loop
						foundEnd = true;
					}
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

		//Add everything found between IFTHEN and ELSE to tkBuffer
		iptr->tkBuffer.splice(iptr->tkBuffer.begin(), ifBuffer);
	}
	else //if the value was false, skip over the tokens inside the if part.
	{
		bool foundElse = false;

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

					if (endIfs > ifThens)
					{
						//no else statement found, end the loop
						foundElse = true;
					}
				}
				else if (next.text == "ELSE" && ifThens <= endIfs)
				{
					foundElse = true;
				}
			}
		}
		while (!foundElse);
	}
}






//Loop operations

void Interpreter::doDO(Interpreter *iptr)
{
	list<Token> buffer;

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
	list<Token> copy = buffer;
	iptr->loopBuffers.push(buffer);
	iptr->tkBuffer.splice(iptr->tkBuffer.begin(), copy);
}

void Interpreter::doUNTIL(Interpreter *iptr)
{
	iptr->checkStackSize(1, "UNTIL");

	Token p = iptr->popStack();

	//Return the tokens to the token buffer if the loop is still going,
	//otherwise empty the list.
	if (!p.value)
	{
		list<Token> copy = iptr->loopBuffers.top();
		iptr->tkBuffer.splice(iptr->tkBuffer.begin(), copy);
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
		cerr << "Error: The identifier \"" << function.text << "\" already exists." << endl;
		return; //TODO: THROW AN EXCEPTION HERE?
	}

	list<Token> tokens;

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
	iptr->params.push(Token(INTEGER, "", rand()));
}



void Interpreter::doINPUT(Interpreter * iptr)
{
	//TODO: WHAT ABOUT STRINGS????
	int input;

	cin >> input;

	iptr->params.push(Token(INTEGER, "", input));
}
