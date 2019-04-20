#include <fstream>
#include "Parser.h"

using namespace std;

Parser::Parser(const string& fileName, const map<string, Symbol>& symbolTable)
{
	symTab = symbolTable;

	ifstream infile(fileName);
	readInput(infile);
}

list<Token> Parser::getTokens()
{
	return tokensRead;
}

void Parser::readInput(ifstream& infile)
{
	string line;
	//TODO: Check for failure to open?
	while (!infile.eof())
	{
		getline(infile, line);
		parseLine(line);
	}
}

void Parser::parseLine(const string& line)
{
	bool endOfLine = false;

	//TODO: Unsigned? is it necessary?
	for (unsigned int i = 0; !endOfLine && i < line.length(); i++)
	{
		//read each character, ignoring whitespace
		if (!isspace(line[i]))
		{
			if (isComment(line, i))
			{
				//ignore comments
				endOfLine = true;
			}
			else
			{
				//when a character is found, read until whitespace is encountered
				unsigned int j = i + 1;
				if (isStringLiteral(line, i)) //Read a string literal
				{
					j = readStringLiteral(line, i);
				}
				else //Read a normal token
				{
					j = readSingleToken(line, i);
				}

				//Skip over already read chars
				i = j;
			}
		}
	}
}

Token Parser::readString(const string& str)
{
	Token t;

	//TODO: FIND VARIABLES????
	//TODO: FIND FUNCTIONS BEFORE RUN-TIME?

	//Check the symbol table
	//map<string, Symbol>::iterator it = symTab.find(str);
	if (symTab.find(str) != symTab.end())
	{
		t = Token(symTab[str].getType(), str, symTab[str].getValue());
	}
	else
	{
		t = Token(UNKNOWN, str, 0);
	}

	return t;
}

int Parser::readStringLiteral(const string& line, const int i)
{
	//skip over the (.").
	int start = i + 2;

	//find the location of the end quote
	unsigned int end = start + 1;
	do
	{
		end++;
	}
	while (end < line.length() && line[end] != '\"');

	//TODO: WHAT IF THERE IS A CHAR IMMEDIATELY AFTER THE ENDQUOTE? MAKE IT UNKNOWN? IDK...
	//YOU'D PROBABLY HAVE TO RE-READ ALL THE WHITESPACE AND REDO EVERYTHING IDK...
	//YOU'D PROBABLY HAVE TO RESET THE i VALUE TO ITS ORIGINAL, IN THAT CASE JUST NEVER CHANGE IT???
	if (line[end] == '\"')
	{
		//TODO: TEST THIS WHEN CLOSE TO END OF LINE:
		//Create the substring starting after the (.") and before the (").
		string text = line.substr(start, end - start);
		tokensRead.push_back(Token(text));
	}
	//else //TODO: WHAT DO IF STRING DOESNT HAVE END QUOTES?
	//{
	//	
	//}

	return end;
}

int Parser::readSingleToken(const string & line, const int i)
{
	int start = i;
	unsigned int end = start + 1;

	//Check if the current char is a number or a negative sign preceding a number.
	bool isInt = isStartOfInt(line, i);

	while (end < line.length() && !isspace(line[end]))
	{
		if (isInt)
		{
			//check if each char is a number.
			isInt = isdigit(line[end]);
		}
		end++;
	}

	string encountered = line.substr(start, end - start);

	Token newToken;
	if (isInt)
	{
		newToken = Token(stoi(encountered));
	}
	else
	{
		newToken = readString(encountered);
	}

	tokensRead.push_back(newToken);

	return end;
}

bool Parser::isComment(const string& line, const int i) const
{
	return equalsTwoChars(line, i, '/', '/');
}

bool Parser::isStringLiteral(const string& line, const int i) const
{
	return equalsTwoChars(line, i, '.', '\"');
}

bool Parser::isStartOfInt(const string& line, const int i) const
{
	return isdigit(line[i]) || (line[i] == '-' && !isEndOfLine(line, i) && isdigit(line[i + 1]));
}

bool Parser::equalsTwoChars(const string& line, const int i, const char char1, const char char2) const
{
	return (line[i] == char1) && !isEndOfLine(line, i) && (line[i + 1] == char2);
}

bool Parser::isEndOfLine(const string& line, const int i) const
{
	return i == line.length() - 1;
}
