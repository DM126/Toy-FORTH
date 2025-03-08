#include <string>
#include "Token.h"

Token::Token() = default;

Token::Token(Types type, const std::string& text, int value)
{
	this->type = type;
	this->text = text;
	this->value = value;
}

Token::Token(const std::string & text)
{
	this->type = Types::LITERAL;
	this->text = text;
	this->value = 0;
}

Token::Token(const int value)
{
	this->type = Types::INTEGER;
	this->text = "";
	this->value = value;
}

Types Token::getType() const
{
	return type;
}

std::string Token::getText() const
{
	return text;
}

int Token::getValue() const
{
	return value;
}
