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
	

	//minijson::istream_context ctx(in);
 }