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
	Parser(const std::string& fileName, const std::map<std::string, Symbol>& symbolTable);

	//Return all the tokens read by the parser
	std::list<Token> getTokens();

private:
	//Read through the entire text file line by line (calls parseLine)
	void readInput(std::ifstream& infile);

	//Parses a line of the text file for tokens.
	//Ignores whitespace and comments.
	void parseLine(const std::string& line);

	//Reads a string and determines the type of token or symbol it is.
	Token readString(const std::string& str);

	//Parses a line of text for a string literal beginning at the
	//specified index. Called if the ." pattern is encountered.
	//Adds the string literal to the list of tokens.
	//Returns the ending index within the line.
	int readStringLiteral(const std::string& str, const unsigned int i);

	//Parses a line of text for a single token, ending at the first discovered
	//whitespace. This token could be an integer, variable, or a
	//keyword/operator. (basically anything that isn't a string literal)
	//Adds the token to the list of tokens.
	//Returns the ending index within the line.
	int readSingleToken(const std::string& line, const unsigned int i);

	//Returns true if a comment begins at the specified index of the line
	bool isComment(const std::string& line, const unsigned int i) const;

	//Returns true if the specified index is the beginning of a string literal (.")
	bool isStringLiteral(const std::string& line, const unsigned int i) const;

	//Returns true if the current character is a digit or a negative sign
	//preceding a digit.
	bool isStartOfInt(const std::string& line, const unsigned int i) const;

	//Returns true if the character at index i of line is equal to char1 and
	//the character at the index after i is equal to char2. Will return false
	//if i is at the end of the line.
	bool equalsTwoChars(const std::string& line, const unsigned int i, const char char1, const char char2) const;

	//Returns true if the index is the last character on the line
	bool isEndOfLine(const std::string& line, const unsigned int i) const;

	//Symbol table
	std::map<std::string, Symbol> symTab;

	//List of all non-whitespace/non-comment tokens read by the parser in 
	//the order they were encountered
	std::list<Token> tokensRead;
};

#endif // !PARSER_H
