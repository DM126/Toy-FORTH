#ifndef PARSER_H
#define PARSER_H

#include <map>
#include <list>
#include <string>
#include "Token.h"
#include "Symbol.h"

class Parser
{
public:
	Parser(const string& fileName, const map<string, Symbol>& symbolTable);

	//Return all the tokens read by the parser
	list<Token> getTokens();

private:
	//Read through the entire text file line by line (calls parseLine)
	void readInput(ifstream& infile);

	//Parses a line of the text file for tokens.
	//Ignores whitespace and comments.
	void parseLine(const string& line);

	//Reads a string and determines the type of token or symbol it is.
	Token readString(const string& str);

	//Symbol table
	map<string, Symbol> symTab;

	//List of all non-whitespace/non-comment tokens read by the parser in 
	//the order they were encountered
	list<Token> tokensRead;
};

#endif // !PARSER_H
