#include <fstream>
#include "Parser.h"

Parser::Parser(const std::string& fileName, const std::map<std::string, Symbol>& symbolTable)
{
	symTab = symbolTable;

	std::ifstream infile(fileName);
	if (infile.fail())
	{
		throw std::runtime_error("Error: Could not find file \"" + fileName + "\"");
	}
	
	readInput(infile);
}

std::list<Token> Parser::getTokens()
{
	return tokensRead;
}

void Parser::readInput(std::ifstream& infile)
{
	std::string line;
	
	while (!infile.eof())
	{
		std::getline(infile, line);
		parseLine(line);
	}
}

void Parser::parseLine(const std::string& line)
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

Token Parser::readString(const std::string& str)
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

int Parser::readStringLiteral(const std::string& line, const int i)
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
		std::string text = line.substr(start, end - start);
		tokensRead.push_back(Token(text));
	}
	//else //TODO: WHAT DO IF STRING DOESNT HAVE END QUOTES?
	//{
	//	
	//}

	return end;
}

int Parser::readSingleToken(const std::string & line, const int i)
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

	std::string encountered = line.substr(start, end - start);

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

bool Parser::isComment(const std::string& line, const int i) const
{
	return equalsTwoChars(line, i, '/', '/');
}

bool Parser::isStringLiteral(const std::string& line, const int i) const
{
	return equalsTwoChars(line, i, '.', '\"');
}

bool Parser::isStartOfInt(const std::string& line, const int i) const
{
	return isdigit(line[i]) || (line[i] == '-' && !isEndOfLine(line, i) && isdigit(line[i + 1]));
}

bool Parser::equalsTwoChars(const std::string& line, const int i, const char char1, const char char2) const
{
	return (line[i] == char1) && !isEndOfLine(line, i) && (line[i + 1] == char2);
}

bool Parser::isEndOfLine(const std::string& line, const int i) const
{
	return i == line.length() - 1;
}
