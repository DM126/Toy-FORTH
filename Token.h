#ifndef TOKEN_H
#define TOKEN_H

#include <array>
#include <string>
#include <ostream>

//Types of tokens
enum class Types { KEYWORD, VARIABLE, LITERAL, INTEGER, ARRAY, FUNCTION, UNKNOWN };

//for debugging
const std::array<std::string, 7> typeArray = { "KEYWORD", "VARIABLE", "LITERAL", "INTEGER", "ARRAY", "FUNCTION", "UNKNOWN" };

//Tokens represent every item encountered in the input file.
class Token
{
public:
	Token();

	//Constructor for an unknown/variable/array token
	Token(Types type, const std::string& text, const int value);

	//constructor for a string literal token
	explicit Token(const std::string& text);

	//constructor for an integer token
	explicit Token(const int value);

	Types getType() const;
	std::string getText() const;
	int getValue() const;

	//overloaded output operator
	friend std::ostream& operator<<(std::ostream& outstream, const Token& t){
		outstream << typeArray[static_cast<int>(t.type)] << " \"" << t.text << "\" " << t.value << std::endl;
	
		return outstream;
	};

private:
	//The type of token determined by the Types enum
	Types type;

	//the text this token was created with
	std::string text;

	//the value of this token. Strings have a value of 0.
	int value;

	friend class Interpreter;
};

#endif
