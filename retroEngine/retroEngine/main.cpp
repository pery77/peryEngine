#include "engine.h"

int main(int argc, char* argv[])
{

	//Create engine and start.
	pery::Engine* engine = new pery::Engine();
	engine->Go();

	delete engine;  //Deallocate engine from memory

#ifdef _DEBUG
	LOG("Press ENTER to exit.");
	std::cin.ignore(1); //Wait for enter key in debug mode.
#endif
	
	return 0;		//Exit program
}