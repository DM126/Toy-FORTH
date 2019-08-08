#include <string>
#include "Token.h"

using namespace std;

Token::Token()
{
	//Intentionally left blank.
}

Token::Token(Types type, const string& text, int value)
{
	this->type = type;
	this->text = text;
	this->value = value;
}

Token::Token(const std::string & text)
{
	this->type = LITERAL;
	this->text = text;
	this->value = 0;
}

Token::Token(const int value)
{
	this->type = INTEGER;
	this->text = "";
	this->value = value;
}

Types Token::getType()
{
	return type;
}

string Token::getText()
{
	return text;
}

int Token::getValue()
{
	return value;
}

//overloaded output operator for Token
ostream & operator<<(ostream& outstream, const Token& t)
{
	outstream << typeArray[t.type] << " \"" << t.text << "\" " << t.value << endl;

	return outstream;
}
