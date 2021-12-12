#include <string>
#include "Token.h"

Token::Token()
{
	//Intentionally left blank.
}

Token::Token(Types type, const std::string& text, int value)
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

std::string Token::getText()
{
	return text;
}

int Token::getValue()
{
	return value;
}

//overloaded output operator for Token
std::ostream & operator<<(std::ostream& outstream, const Token& t)
{
	outstream << typeArray[t.type] << " \"" << t.text << "\" " << t.value << std::endl;

	return outstream;
}
