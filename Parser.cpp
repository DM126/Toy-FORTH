#include <fstream>
#include <sstream>
#include "Parser.h"

Parser::Parser(const std::string& fileName, const std::map<std::string, Symbol, std::less<>>& symbolTable)
{
	symTab = symbolTable;

	std::ifstream infile(fileName);
	if (infile.fail())
	{
		throw std::runtime_error("Parser error: Could not find file \"" + fileName + "\"");
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

	for (unsigned int i = 0; !endOfLine && i < line.length(); i++)
	{
		//read each character, ignoring whitespace
		if (!std::isspace(line[i]))
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

int Parser::readStringLiteral(const std::string& line, const unsigned int i)
{
	std::stringstream ss;

	//skip over the (.").
	int start = i + 2;

	//TODO refactor this
	//find the location of the end quote (escaped quotes are allowed)
	unsigned int end = start;
	bool escaped = false;
	while (end < line.length() && !(line[end] == '\"' && !escaped))
	{
		char currentChar = line[end];

		//Check if beginning an escape character
		if (!escaped && currentChar == '\\')
		{
			escaped = true;
		}
		else
		{
			//Add the character or an escape character
			if (escaped)
			{
				switch (currentChar)
				{
					case '\"':
					case '\\': ss << currentChar; break;
					default: throw std::runtime_error("Parser error: '" + std::string(1, currentChar) + "' is not an escape character");
				}
			}
			else
			{
				ss << currentChar;
			}

			escaped = false;
		}

		end++;
	}

	if (line[end] == '\"')
	{
		//String literals must be followed by whitespace
		if (end + 1 < line.length() && !std::isspace(line[end + 1]))
		{
			throw std::runtime_error("Parser error: string literals can only be immediately followed by whitespace");
		}

		//Create the string starting after the (.") and before the (").
		std::string text = ss.str();
		tokensRead.emplace_back(Token(text));
	}
	else
	{
		throw std::runtime_error("Parser error: String literal not closed");
	}

	return end;
}

int Parser::readSingleToken(const std::string & line, const unsigned int i)
{
	unsigned int start = i;
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
		try
		{
			newToken = Token(stoi(encountered));
		}
		catch (std::out_of_range&)
		{
			throw std::runtime_error("Parser error: int value " + encountered + " is too large");
		}
	}
	else
	{
		newToken = readString(encountered);
	}

	tokensRead.push_back(newToken);

	return end;
}

bool Parser::isComment(const std::string& line, const unsigned int i) const
{
	return equalsTwoChars(line, i, '/', '/');
}

bool Parser::isStringLiteral(const std::string& line, const unsigned int i) const
{
	return equalsTwoChars(line, i, '.', '\"');
}

bool Parser::isStartOfInt(const std::string& line, const unsigned int i) const
{
	return std::isdigit(line[i]) || (line[i] == '-' && !isEndOfLine(line, i) && std::isdigit(line[i + 1]));
}

bool Parser::equalsTwoChars(const std::string& line, const unsigned int i, const char char1, const char char2) const
{
	return (line[i] == char1) && !isEndOfLine(line, i) && (line[i + 1] == char2);
}

bool Parser::isEndOfLine(const std::string& line, const unsigned int i) const
{
	return i == line.length() - 1;
}
