#include "engine.h"
#include <iostream>
#include <fstream>
#include "minijson_reader.hpp"
using namespace std;

int main(int argc, char* argv[])
{
	const char *fname = "../Assets/test.json";
	std::ifstream in(fname);

	
	minijson::istream_context ctx(in);

	minijson::parse_object(ctx, [&](const char *name, minijson::value v) {
		minijson::dispatch(name)
			<< "version" >>      [&] { std::cout << "version: " << v.as_double() << endl; }
			<< "height" >> [&] { cout << "height: " << v.as_double() << endl; }
			<< "width" >> [&] { cout << "width: " << v.as_double() << endl; }
			<< "layers" >> [&] {
				cout << "layers-> " << endl;
				minijson::parse_array(ctx, [&](minijson::value va)
				{
					cout << "layer: " << endl;
					minijson::parse_object(ctx, [&](const char *name, minijson::value va) {
						minijson::dispatch(name)
							<< "name" >> [&] { std::cout << "name: " << va.as_string() << endl; };
						minijson::ignore(ctx);
					});
				});
				cout << endl; 
				minijson::ignore(ctx); 
			}

		<< minijson::any >> [&] { minijson::ignore(ctx); };
	});

	//Create engine and start.
//	Engine* engine = new Engine();
//	engine->Go();

	std::cin.get(); //Wait key
	return 0;
}