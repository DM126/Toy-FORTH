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
		string encountered;

		//read each character, ignoring whitespace
		if (!isspace(line[i]))
		{
			if (line[i] == '/' && i + 1 < line.length() && line[i + 1] == '/')
			{
				//ignore comments
				endOfLine = true;
			}
			else
			{
				//when a character is found, read until whitespace is encountered
				unsigned int j = i + 1;
				if (line[i] == '.' && j < line.length() && line[j] == '\"') //Read a string literal
				{
					//skip over the (.").
					i += 2;

					do
					{
						j++;
					}
					while (j < line.length() && line[j] != '\"');

					//TODO: WHAT IF THERE IS A CHAR IMMEDIATELY AFTER THE ENDQUOTE? MAKE IT UNKNOWN? IDK...
					//YOU'D PROBABLY HAVE TO RE-READ ALL THE WHITESPACE AND REDO EVERYTHING IDK...
					//YOU'D PROBABLY HAVE TO RESET THE i VALUE TO ITS ORIGINAL, IN THAT CASE JUST NEVER CHANGE IT???
					if (line[j] == '\"')
					{
						//TODO: TEST THIS WHEN CLOSE TO END OF LINE:
						//Create the substring starting after the (.") and before the (").
						tokensRead.push_back(Token(LITERAL, line.substr(i, j - i), 0));
					}
					/*else //TODO: WHAT DO IF STRING DOESNT HAVE END QUOTES?
					{

					}*/
				}
				else //Read a normal token
				{
					//Check if the current char is a number or a negative sign preceding a number.
					bool isInt = (isdigit(line[i]) || (line[i] == '-' && j < line.length() && isdigit(line[j])));

					while (j < line.length() && !isspace(line[j]))
					{
						if (isInt)
						{
							//check if each char is a number.
							isInt = isdigit(line[j]);
						}
						j++;
					}

					encountered = line.substr(i, j - i);

					Token newToken;
					if (isInt)
					{
						newToken = Token(INTEGER, "", stoi(encountered));
					}
					else
					{
						newToken = readString(encountered);
					}

					tokensRead.push_back(newToken);
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
