#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <ostream>

using namespace std;

//Types of tokens
enum Types { KEYWORD, VARIABLE, LITERAL, INTEGER, FUNCTION, UNKNOWN };

//for debugging
const string typeArray[] = { "KEYWORD", "VARIABLE", "LITERAL", "INTEGER", "FUNCTION", "UNKNOWN" };

//Tokens represent every item encountered in the input file.
class Token
{
public:
	Token();

	//constructor
	Token(Types type, const string& text, int value);

	Types getType();
	string getText();
	int getValue();

	//overloaded output operator
	friend ostream& operator<<(ostream& outstream, const Token& t);

private:
	//The type of token determined by the Types enum
	Types type;

	//the text this token was created with
	string text;

	//the value of this token
	int value;

	friend class Interpreter;
};

#endif // !TOKEN_H
