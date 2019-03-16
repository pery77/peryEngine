#include "tools.h"
#include "raylib.h"
#include <sstream>

using namespace pery;

void Tools::Log(const char* text)
{
	std::cout << text << std::endl;
}
void Tools::Log(int text)
{
	std::cout << text << std::endl;
}
void Tools::Log(std::string text)
{
	std::cout << text << std::endl;
}

static inline Rectangle colourFromString(std::string str)
{
	//removes preceding #
	auto result = str.find_last_of('#');
	if (result != std::string::npos)
	{
		str = str.substr(result + 1);
	}

	if (str.size() == 6 || str.size() == 8)
	{
		unsigned int value, r, g, b;
		unsigned int a = 255;
		std::stringstream input(str);
		input >> std::hex >> value;

		r = (value >> 16) & 0xff;
		g = (value >> 8) & 0xff;
		b = value & 0xff;

		if (str.size() == 8)
		{
			a = (value >> 24) & 0xff;
		}

		return { (float)r, (float)g, (float)b, (float)a };
	}
}