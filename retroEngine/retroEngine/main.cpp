#include "engine.h"
#include <iostream>
#include <fstream>
#include "minijson_reader.hpp"
using namespace std;

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

	//Create engine and start.
	Engine* engine = new Engine();
	engine->Go();

	//std::cin.get(); //Wait key
	return 0;
}