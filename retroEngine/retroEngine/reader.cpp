#include "reader.h"

pery::Reader::Reader()
{
}

pery::Reader::~Reader() 
{
}
void pery::Reader::LoadFile(std::string fileName)
{
	std::string st = "../Assets/" + fileName + ".json";

	//Load file
	const char *fname = st.c_str();
	std::ifstream in(fname);


	//Error loading file
	if (in.fail())
	{
		LOG("[ERROR] Loading:");
		LOG(fname);
		return;
	}

	std::string str((std::istreambuf_iterator<char>(in)),
		std::istreambuf_iterator<char>());

	//Succeful load file
	LOG("Loading:");
	LOG(fname);
	static int parsed;
	
	char *source = _strdup(str.c_str());
	char *endptr;
	JsonValue value;
	JsonAllocator allocator;
	int result = jsonParse(source, &endptr, &value, allocator);
	if (result) {
		fprintf(stderr, "FAILED %d: %s\n%s\n%*s - \\x%02X\n", parsed, jsonStrError(result), st.c_str() , (int)(endptr - source + 1), "^", *endptr);
	}

	switch (value.getTag()) {
	
	case JSON_ARRAY:
		for (auto i : value) {
			printf(" array %s = %g\n", i->key, i->value);
		}
		break;
	case JSON_OBJECT:
		for (auto i : value) {
			printf("%s = %g\n", i->key, i->value);
		}
		break;

	}

	//minijson::istream_context ctx(in);
 }