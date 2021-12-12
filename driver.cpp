#include <iostream>
#include "Interpreter.h"

int main(int argc, char* argv[])
{
	//Validate the number of command line arguments
	if (argc != 2)
	{
		std::cerr << "Error: Interpreter takes exactly 1 argument for the file to execute." << std::endl;
		return 1;
	}

	//Convert the filename argument to a string and run the interpreter
	std::string filename = std::string(argv[1]);
	Interpreter interpreter(filename);

	return 0;
}
