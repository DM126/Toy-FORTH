#include <iostream>
#include "Interpreter.h"

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cerr << "Error: Interpreter takes exactly 1 argument for the file to execute." << std::endl;
		return 1;
	}

	std::string filename = std::string(argv[1]);
	Interpreter interpreter(filename);

	return 0;
}
