#include <iostream> 	// std::cout
#include <tpl/regex/Match.h> // minimization of including tpl

using namespace tpl;

// -------------------------------------------------------------------------

void validation_example_1(const char* str)
{
	if (str == integer())
		std::cout << "  " << str << " is an integer value.\n";
	else if (str == real())
		std::cout << "  " << str << " is a real value.\n";
	else
		std::cout << "  " << str << " isn't a numeric value.\n";
}

// -------------------------------------------------------------------------

int main()
{
	validation_example_1("-135");
	validation_example_1("+.23e-23");
	validation_example_1("-.1.e23");
	return 0;
}

// -------------------------------------------------------------------------

