#include "engine.h"
#include <iostream>

int main(int argc, char* argv[])
{
	//Create engine and start.
	Engine* engine = new Engine();
	engine->Go();

//	std::cin.get(); //Wait key
	return 0;
}