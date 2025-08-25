#include <iostream>
#include <magic_enum/magic_enum.hpp>

enum class Color : int { RED = -10, BLUE = 0, GREEN = 10 };

int main()
{
	std::cout << "Hello, World!!!\n";

	Color c1 = Color::RED;
	std::cout << magic_enum::enum_name(c1) << std::endl; // RED

	return 0;
}