#include "engine.h"
#include "reader.h"

int main(int argc, char* argv[])
{/*
	//Load file
	const char *fname = "../Assets/config.json";
	std::ifstream in(fname);	
	minijson::istream_context ctx(in);

	//Main parse
	minijson::parse_object(ctx, [&](const char *name, minijson::value v) {
		minijson::dispatch(name)
			<< "height" >>  [&] { cout << "height: " << v.as_double() << endl; }
			<< "width" >>   [&] { cout << "width: " << v.as_double() << endl; }
			
		<< minijson::any >> [&] { minijson::ignore(ctx); };
	});*/

	pery::Reader* reader = new pery::Reader();
	reader->LoadFile("big");

	//Create engine and start.
	pery::Engine* engine = new pery::Engine();
//	engine->Go();

	delete engine;  //Deallocate engine from memory
	delete reader;
#ifdef _DEBUG
	LOG("Press ENTER to exit.");
	std::cin.ignore(1); //Wait for enter key in debug mode.
#endif
	
	return 0;		//Exit program
}